#pragma once

#include <yetty/result.hpp>
#include <memory>
#include <string>

namespace yetty {

/**
 * SharedMemoryRegion provides cross-process shared memory access.
 *
 * Used by CardManager to share buffer/texture backing memory with clients.
 * The shm region IS the backing memory for CardBufferManager/CardTextureManager,
 * enabling zero-copy streaming from client to GPU.
 *
 * Usage:
 *   Server: auto shm = SharedMemoryRegion::create("yetty-shm-data-1234", 16*1024*1024);
 *   Client: auto shm = SharedMemoryRegion::open("yetty-shm-data-1234");
 *   Both:   void* ptr = shm->data();  // Same physical memory
 */
class SharedMemoryRegion {
public:
    using Ptr = std::shared_ptr<SharedMemoryRegion>;

    /**
     * Create a new shared memory region (server side).
     * @param name Unique name for the region (e.g., "yetty-shm-data-<pid>")
     * @param initialSize Initial size in bytes
     * @return Shared pointer to region or error
     */
    static Result<Ptr> create(const std::string& name, size_t initialSize) noexcept;

    /**
     * Open an existing shared memory region (client side).
     * @param name Name of the region to open
     * @return Shared pointer to region or error
     */
    static Result<Ptr> open(const std::string& name) noexcept;

    ~SharedMemoryRegion();

    // Non-copyable, non-movable (due to mmap)
    SharedMemoryRegion(const SharedMemoryRegion&) = delete;
    SharedMemoryRegion& operator=(const SharedMemoryRegion&) = delete;
    SharedMemoryRegion(SharedMemoryRegion&&) = delete;
    SharedMemoryRegion& operator=(SharedMemoryRegion&&) = delete;

    /**
     * Get pointer to the mapped memory.
     * @return Pointer to the start of the shared memory region
     */
    void* data() const { return _mapping; }

    /**
     * Get current size of the region.
     * @return Size in bytes
     */
    size_t size() const { return _size; }

    /**
     * Get the region name.
     * @return Name used to create/open the region
     */
    const std::string& name() const { return _name; }

    /**
     * Grow the shared memory region.
     * After calling this, the old mapping is invalidated and clients must remap.
     * @param newSize New size in bytes (must be > current size)
     * @return Ok or error
     */
    Result<void> grow(size_t newSize) noexcept;

    /**
     * Remap after the region was grown by another process.
     * Client calls this after receiving a grow notification.
     * @return Ok or error
     */
    Result<void> remap() noexcept;

    /**
     * Check if this is the owner (creator) of the region.
     * Only the owner can grow the region.
     */
    bool isOwner() const { return _isOwner; }

private:
    SharedMemoryRegion(std::string name, int fd, void* mapping, size_t size, bool isOwner);

    std::string _name;
    int _fd;            // shm_open file descriptor
    void* _mapping;     // mmap'd address
    size_t _size;       // current size
    bool _isOwner;      // true if we created it (can grow/unlink)
};

} // namespace yetty
