#include <yetty/card-manager.h>
#include <ytrace/ytrace.hpp>
#include <array>

#define WGPU_STR(s) WGPUStringView{.data = s, .length = WGPU_STRLEN}

namespace yetty {

class CardManagerImpl : public CardManager {
public:
    CardManagerImpl(GPUContext* gpuContext, WGPUBuffer uniformBuffer, uint32_t uniformSize, Config config)
        : _gpuContext(gpuContext)
        , _device(gpuContext->device)
        , _uniformBuffer(uniformBuffer)
        , _uniformSize(uniformSize)
        , _config(config) {}

    Result<void> init() noexcept;
    ~CardManagerImpl() override;

    // Manager accessors
    CardBufferManager::Ptr bufferManager() const override { return _bufferMgr; }
    CardTextureManager::Ptr textureManager() const override { return _textureMgr; }

    // Bind group
    WGPUBindGroupLayout sharedBindGroupLayout() const override { return _sharedBindGroupLayout; }
    WGPUBindGroup sharedBindGroup() const override { return _sharedBindGroup; }
    Result<void> updateBindGroup() override;

    // GPU upload coordination
    Result<void> flush(WGPUQueue queue) override;

private:
    Result<void> createDummyResources();
    Result<void> createSharedBindGroup();

    GPUContext* _gpuContext;
    WGPUDevice _device;
    WGPUBuffer _uniformBuffer;
    uint32_t _uniformSize;
    Config _config;

    CardBufferManager::Ptr _bufferMgr;
    CardTextureManager::Ptr _textureMgr;

    // Bind group
    WGPUBindGroupLayout _sharedBindGroupLayout = nullptr;
    WGPUBindGroup _sharedBindGroup = nullptr;
    bool _bindGroupHasRealAtlas = false;

    // Dummy resources for bind group before atlas init
    WGPUTexture _dummyAtlasTexture = nullptr;
    WGPUTextureView _dummyAtlasTextureView = nullptr;
    WGPUSampler _dummyAtlasSampler = nullptr;
};

Result<CardManager::Ptr> CardManager::create(GPUContext* gpuContext,
                                              WGPUBuffer uniformBuffer,
                                              uint32_t uniformSize,
                                              Config config) noexcept {
    auto mgr = std::make_shared<CardManagerImpl>(gpuContext, uniformBuffer, uniformSize, config);
    if (auto res = mgr->init(); !res) {
        return Err<Ptr>("CardManager init failed", res);
    }
    return Ok(std::static_pointer_cast<CardManager>(mgr));
}

Result<void> CardManagerImpl::init() noexcept {
    // Create texture manager first (buffer manager may need it for type checking)
    auto texRes = CardTextureManager::create(_gpuContext, _config.texture);
    if (!texRes) {
        return Err<void>("Failed to create CardTextureManager", texRes);
    }
    _textureMgr = *texRes;

    // Create buffer manager (no longer owns bind group)
    auto bufRes = CardBufferManager::create(_gpuContext, _textureMgr,
                                            _uniformBuffer, _uniformSize, _config.buffer);
    if (!bufRes) {
        return Err<void>("Failed to create CardBufferManager", bufRes);
    }
    _bufferMgr = *bufRes;

    // Create dummy resources for bind group
    if (auto res = createDummyResources(); !res) {
        return Err<void>("Failed to create dummy resources", res);
    }

    // Create initial bind group
    if (auto res = createSharedBindGroup(); !res) {
        return Err<void>("Failed to create shared bind group", res);
    }

    yinfo("CardManager: initialized with buffer and texture managers");
    return Ok();
}

CardManagerImpl::~CardManagerImpl() {
    if (_sharedBindGroup) wgpuBindGroupRelease(_sharedBindGroup);

    if (_dummyAtlasSampler) wgpuSamplerRelease(_dummyAtlasSampler);
    if (_dummyAtlasTextureView) wgpuTextureViewRelease(_dummyAtlasTextureView);
    if (_dummyAtlasTexture) wgpuTextureRelease(_dummyAtlasTexture);
}

Result<void> CardManagerImpl::createDummyResources() {
    // 1x1 RGBA8 texture as placeholder for atlas texture binding
    WGPUTextureDescriptor texDesc = {};
    texDesc.label = WGPU_STR("dummy atlas");
    texDesc.size = {1, 1, 1};
    texDesc.format = WGPUTextureFormat_RGBA8Unorm;
    texDesc.usage = WGPUTextureUsage_TextureBinding;
    texDesc.dimension = WGPUTextureDimension_2D;
    texDesc.mipLevelCount = 1;
    texDesc.sampleCount = 1;
    _dummyAtlasTexture = wgpuDeviceCreateTexture(_device, &texDesc);
    if (!_dummyAtlasTexture) {
        return Err<void>("Failed to create dummy atlas texture");
    }

    WGPUTextureViewDescriptor viewDesc = {};
    viewDesc.format = WGPUTextureFormat_RGBA8Unorm;
    viewDesc.dimension = WGPUTextureViewDimension_2D;
    viewDesc.mipLevelCount = 1;
    viewDesc.arrayLayerCount = 1;
    _dummyAtlasTextureView = wgpuTextureCreateView(_dummyAtlasTexture, &viewDesc);
    if (!_dummyAtlasTextureView) {
        return Err<void>("Failed to create dummy atlas texture view");
    }

    WGPUSamplerDescriptor samplerDesc = {};
    samplerDesc.minFilter = WGPUFilterMode_Linear;
    samplerDesc.magFilter = WGPUFilterMode_Linear;
    samplerDesc.addressModeU = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeV = WGPUAddressMode_ClampToEdge;
    samplerDesc.addressModeW = WGPUAddressMode_ClampToEdge;
    samplerDesc.maxAnisotropy = 1;
    _dummyAtlasSampler = wgpuDeviceCreateSampler(_device, &samplerDesc);
    if (!_dummyAtlasSampler) {
        return Err<void>("Failed to create dummy atlas sampler");
    }

    return Ok();
}

Result<void> CardManagerImpl::createSharedBindGroup() {
    if (_sharedBindGroup) {
        wgpuBindGroupRelease(_sharedBindGroup);
        _sharedBindGroup = nullptr;
    }

    if (!_sharedBindGroupLayout) {
        _sharedBindGroupLayout = _gpuContext->sharedBindGroupLayout;
        if (!_sharedBindGroupLayout) {
            return Err<void>("No shared bind group layout in GPUContext");
        }
    }

    // Use real atlas if initialized, otherwise dummy
    bool atlasReady = _textureMgr && _textureMgr->atlasInitialized();
    WGPUTextureView texView = atlasReady ? _textureMgr->atlasTextureView() : _dummyAtlasTextureView;
    WGPUSampler sampler = atlasReady ? _textureMgr->atlasSampler() : _dummyAtlasSampler;

    if (!texView || !sampler) {
        return Err<void>("Missing texture view or sampler for bind group");
    }

    std::array<WGPUBindGroupEntry, 6> bindEntries = {};

    // Binding 0: uniform buffer
    bindEntries[0].binding = 0;
    bindEntries[0].buffer = _uniformBuffer;
    bindEntries[0].size = _uniformSize;

    // Binding 1: metadata buffer (from buffer manager)
    bindEntries[1].binding = 1;
    bindEntries[1].buffer = _bufferMgr->metadataBuffer();
    bindEntries[1].size = wgpuBufferGetSize(_bufferMgr->metadataBuffer());

    // Binding 2: buffer (from buffer manager)
    bindEntries[2].binding = 2;
    bindEntries[2].buffer = _bufferMgr->bufferGpu();
    bindEntries[2].size = wgpuBufferGetSize(_bufferMgr->bufferGpu());

    // Binding 3: atlas texture view (from texture manager or dummy)
    bindEntries[3].binding = 3;
    bindEntries[3].textureView = texView;

    // Binding 4: atlas sampler (from texture manager or dummy)
    bindEntries[4].binding = 4;
    bindEntries[4].sampler = sampler;

    // Binding 5: texture buffer (from texture manager)
    bindEntries[5].binding = 5;
    bindEntries[5].buffer = _textureMgr->textureBuffer();
    bindEntries[5].size = wgpuBufferGetSize(_textureMgr->textureBuffer());

    WGPUBindGroupDescriptor bindDesc = {};
    bindDesc.label = WGPU_STR("Shared Bind Group");
    bindDesc.layout = _sharedBindGroupLayout;
    bindDesc.entryCount = bindEntries.size();
    bindDesc.entries = bindEntries.data();

    _sharedBindGroup = wgpuDeviceCreateBindGroup(_device, &bindDesc);
    if (!_sharedBindGroup) {
        return Err<void>("Failed to create shared bind group");
    }

    _bindGroupHasRealAtlas = atlasReady;
    _gpuContext->sharedBindGroup = _sharedBindGroup;

    yinfo("CardManager: bind group created (atlas={})", atlasReady ? "real" : "dummy");
    return Ok();
}

Result<void> CardManagerImpl::updateBindGroup() {
    bool atlasReady = _textureMgr && _textureMgr->atlasInitialized();
    bool bufferDirty = _bufferMgr && _bufferMgr->bindGroupDirty();

    // Recreate if atlas state changed OR buffer was recreated
    if (atlasReady != _bindGroupHasRealAtlas || bufferDirty) {
        if (_bufferMgr) _bufferMgr->clearBindGroupDirty();
        return createSharedBindGroup();
    }
    return Ok();
}

Result<void> CardManagerImpl::flush(WGPUQueue queue) {
    // Pack atlas first (may trigger lazy init)
    if (_textureMgr) {
        if (auto res = _textureMgr->packAtlas(queue); !res) {
            return Err<void>("CardManager: atlas pack failed", res);
        }
        if (auto res = _textureMgr->writeAllToAtlas(queue); !res) {
            return Err<void>("CardManager: atlas write failed", res);
        }
    }

    // Update bind group if atlas was just initialized
    if (auto res = updateBindGroup(); !res) {
        return Err<void>("CardManager: bind group update failed", res);
    }

    // Flush buffer manager
    if (_bufferMgr) {
        if (auto res = _bufferMgr->flush(queue); !res) {
            return Err<void>("CardManager: buffer flush failed", res);
        }
    }

    return Ok();
}

}  // namespace yetty
