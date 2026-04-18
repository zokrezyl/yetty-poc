#pragma once

#include <atomic>
#include <cstdint>
#include <string>

namespace yetty {
namespace rpc {

/**
 * Per-allocation header in shared memory for seqlock synchronization.
 *
 * This header is placed at the start of each allocation in the shared memory
 * region, enabling lock-free coordination between client writes and server
 * GPU uploads.
 *
 * Seqlock protocol:
 *   Client write:
 *     1. Wait while uploading flag is set
 *     2. Increment seq (odd = writing)
 *     3. Write data
 *     4. Increment seq (even = done)
 *
 *   Server read (flush to GPU):
 *     1. Set uploading flag
 *     2. Wait while seq is odd (client writing)
 *     3. Read data / upload to GPU
 *     4. Clear uploading flag
 */
struct AllocationHeader {
    std::atomic<uint32_t> seq;        // odd = write in progress, even = stable
    std::atomic<uint32_t> uploading;  // 1 = server uploading, client must wait
    uint32_t size;                    // allocation size (excluding header)
    uint32_t _pad;                    // alignment padding
};

// Verify lock-free atomics work in shared memory (required for cross-process)
static_assert(std::atomic<uint32_t>::is_always_lock_free,
              "uint32_t atomics must be lock-free for shared memory");

// Verify header is 16 bytes for alignment
static_assert(sizeof(AllocationHeader) == 16,
              "AllocationHeader must be 16 bytes");

/**
 * Handle for a buffer allocation in shared memory.
 * Returned by server via RPC when client queries for a card's buffer.
 */
struct StreamBufferHandle {
    uint32_t offset;    // Offset in shm-data (points to AllocationHeader)
    uint32_t size;      // Size of data (excluding header)

    bool isValid() const { return size > 0; }
    static StreamBufferHandle invalid() { return {0, 0}; }

    // Get pointer to header
    AllocationHeader* header(void* shmBase) const {
        return reinterpret_cast<AllocationHeader*>(
            static_cast<uint8_t*>(shmBase) + offset);
    }

    // Get pointer to data (after header)
    void* data(void* shmBase) const {
        return static_cast<uint8_t*>(shmBase) + offset + sizeof(AllocationHeader);
    }
};

/**
 * Handle for a texture allocation in shared memory.
 */
struct StreamTextureHandle {
    uint32_t offset;    // Offset in shm-tex (points to AllocationHeader)
    uint32_t width;
    uint32_t height;

    bool isValid() const { return width > 0 && height > 0; }
    static StreamTextureHandle invalid() { return {0, 0, 0}; }

    // Size of pixel data (RGBA8)
    uint32_t pixelDataSize() const { return width * height * 4; }

    // Get pointer to header
    AllocationHeader* header(void* shmBase) const {
        return reinterpret_cast<AllocationHeader*>(
            static_cast<uint8_t*>(shmBase) + offset);
    }

    // Get pointer to pixel data (after header)
    void* data(void* shmBase) const {
        return static_cast<uint8_t*>(shmBase) + offset + sizeof(AllocationHeader);
    }
};

/**
 * Shared memory region info returned by stream_connect RPC.
 */
struct ShmRegionInfo {
    std::string name;
    size_t size;
};

/**
 * Seqlock helper functions for client-side use.
 */
namespace seqlock {

/**
 * Begin a write to an allocation. Call before writing data.
 * This function will spin-wait if the server is currently uploading.
 */
inline void beginWrite(AllocationHeader* h) {
    // Wait for any upload to complete
    while (h->uploading.load(std::memory_order_acquire)) {
        // Spin wait - could add yield here for long waits
    }
    // Mark write in progress (seq becomes odd)
    h->seq.fetch_add(1, std::memory_order_release);
}

/**
 * End a write to an allocation. Call after writing data.
 */
inline void endWrite(AllocationHeader* h) {
    // Mark write complete (seq becomes even)
    h->seq.fetch_add(1, std::memory_order_release);
}

/**
 * Begin reading an allocation for GPU upload. Call before reading.
 * Sets the uploading flag and waits for any in-progress write.
 */
inline void beginRead(AllocationHeader* h) {
    h->uploading.store(1, std::memory_order_release);
    // Wait for any in-progress write to complete
    while (h->seq.load(std::memory_order_acquire) & 1) {
        // Spin wait
    }
}

/**
 * End reading an allocation. Call after GPU upload.
 */
inline void endRead(AllocationHeader* h) {
    h->uploading.store(0, std::memory_order_release);
}

} // namespace seqlock

} // namespace rpc
} // namespace yetty
