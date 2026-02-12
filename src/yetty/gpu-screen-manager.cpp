#include <yetty/gpu-screen-manager.h>
#include <algorithm>
#include <mutex>

namespace yetty {

class GPUScreenManagerImpl : public GPUScreenManager {
public:
    void registerScreen(GPUScreenPtr screen) override {
        std::lock_guard<std::mutex> lock(_mutex);
        _screens.push_back(screen);
    }

    void unregisterScreen(GPUScreen* screen) override {
        std::lock_guard<std::mutex> lock(_mutex);
        _screens.erase(
            std::remove_if(_screens.begin(), _screens.end(),
                [screen](const std::weak_ptr<GPUScreen>& wp) {
                    auto sp = wp.lock();
                    return !sp || sp.get() == screen;
                }),
            _screens.end()
        );
    }

    std::vector<GPUScreenPtr> screens() const override {
        std::lock_guard<std::mutex> lock(_mutex);
        std::vector<GPUScreenPtr> result;
        for (const auto& wp : _screens) {
            if (auto sp = wp.lock()) {
                result.push_back(sp);
            }
        }
        return result;
    }

private:
    mutable std::mutex _mutex;
    std::vector<std::weak_ptr<GPUScreen>> _screens;
};

Result<GPUScreenManager::Ptr> GPUScreenManager::createImpl(ContextType& ctx) {
    (void)ctx;
    return Ok<Ptr>(std::make_shared<GPUScreenManagerImpl>());
}

}  // namespace yetty
