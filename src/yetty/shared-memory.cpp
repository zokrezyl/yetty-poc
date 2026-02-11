#include <yetty/shared-memory.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace yetty {

SharedMemoryRegion::SharedMemoryRegion(std::string name, int fd, void* mapping, size_t size, bool isOwner)
    : _name(std::move(name))
    , _fd(fd)
    , _mapping(mapping)
    , _size(size)
    , _isOwner(isOwner) {}

SharedMemoryRegion::~SharedMemoryRegion() {
    if (_mapping && _mapping != MAP_FAILED) {
        munmap(_mapping, _size);
    }
    if (_fd >= 0) {
        close(_fd);
    }
    if (_isOwner) {
        // Owner unlinks the shm on destruction
        shm_unlink(_name.c_str());
    }
}

Result<SharedMemoryRegion::Ptr> SharedMemoryRegion::create(const std::string& name, size_t initialSize) noexcept {
    // Create shared memory object
    int fd = shm_open(name.c_str(), O_CREAT | O_RDWR | O_EXCL, 0600);
    if (fd < 0) {
        if (errno == EEXIST) {
            // Already exists, try to unlink and recreate
            shm_unlink(name.c_str());
            fd = shm_open(name.c_str(), O_CREAT | O_RDWR | O_EXCL, 0600);
        }
        if (fd < 0) {
            return Err<SharedMemoryRegion::Ptr>("shm_open failed: " + std::string(strerror(errno)));
        }
    }

    // Set size
    if (ftruncate(fd, static_cast<off_t>(initialSize)) < 0) {
        close(fd);
        shm_unlink(name.c_str());
        return Err<SharedMemoryRegion::Ptr>("ftruncate failed: " + std::string(strerror(errno)));
    }

    // Map into address space
    void* mapping = mmap(nullptr, initialSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapping == MAP_FAILED) {
        close(fd);
        shm_unlink(name.c_str());
        return Err<SharedMemoryRegion::Ptr>("mmap failed: " + std::string(strerror(errno)));
    }

    // Zero-initialize
    std::memset(mapping, 0, initialSize);

    return Ok(Ptr(new SharedMemoryRegion(name, fd, mapping, initialSize, true)));
}

Result<SharedMemoryRegion::Ptr> SharedMemoryRegion::open(const std::string& name) noexcept {
    // Open existing shared memory object
    int fd = shm_open(name.c_str(), O_RDWR, 0600);
    if (fd < 0) {
        return Err<SharedMemoryRegion::Ptr>("shm_open failed: " + std::string(strerror(errno)));
    }

    // Get current size
    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        close(fd);
        return Err<SharedMemoryRegion::Ptr>("fstat failed: " + std::string(strerror(errno)));
    }
    size_t size = static_cast<size_t>(sb.st_size);

    // Map into address space
    void* mapping = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapping == MAP_FAILED) {
        close(fd);
        return Err<SharedMemoryRegion::Ptr>("mmap failed: " + std::string(strerror(errno)));
    }

    return Ok(Ptr(new SharedMemoryRegion(name, fd, mapping, size, false)));
}

Result<void> SharedMemoryRegion::grow(size_t newSize) noexcept {
    if (!_isOwner) {
        return Err<void>("Only owner can grow shared memory region");
    }
    if (newSize <= _size) {
        return Err<void>("New size must be larger than current size");
    }

    // Unmap old region
    if (munmap(_mapping, _size) < 0) {
        return Err<void>("munmap failed: " + std::string(strerror(errno)));
    }

    // Resize
    if (ftruncate(_fd, static_cast<off_t>(newSize)) < 0) {
        // Try to remap at old size
        _mapping = mmap(nullptr, _size, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
        return Err<void>("ftruncate failed: " + std::string(strerror(errno)));
    }

    // Remap at new size
    _mapping = mmap(nullptr, newSize, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    if (_mapping == MAP_FAILED) {
        return Err<void>("mmap after grow failed: " + std::string(strerror(errno)));
    }

    // Zero the new portion
    std::memset(static_cast<uint8_t*>(_mapping) + _size, 0, newSize - _size);

    _size = newSize;
    return Ok();
}

Result<void> SharedMemoryRegion::remap() noexcept {
    // Get current size from the file
    struct stat sb;
    if (fstat(_fd, &sb) < 0) {
        return Err<void>("fstat failed: " + std::string(strerror(errno)));
    }
    size_t newSize = static_cast<size_t>(sb.st_size);

    if (newSize == _size && _mapping != MAP_FAILED) {
        // No change needed
        return Ok();
    }

    // Unmap old region
    if (_mapping && _mapping != MAP_FAILED) {
        munmap(_mapping, _size);
    }

    // Remap at new size
    _mapping = mmap(nullptr, newSize, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
    if (_mapping == MAP_FAILED) {
        return Err<void>("mmap failed: " + std::string(strerror(errno)));
    }

    _size = newSize;
    return Ok();
}

} // namespace yetty
