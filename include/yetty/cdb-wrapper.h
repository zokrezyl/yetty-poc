#pragma once
//=============================================================================
// CDB Wrapper - Unified interface for Constant Database implementations
//=============================================================================
// Abstracts the differences between:
//   - howerj/cdb: Portable C implementation (default, works on all platforms)
//   - djb/cdb: Original D.J. Bernstein implementation (Unix only)
//
// Usage:
//   // Reading
//   auto reader = CdbReader::open("file.cdb");
//   if (reader) {
//       auto data = reader->get(key, keyLen);
//       if (data) { /* use data->data(), data->size() */ }
//   }
//
//   // Writing
//   auto writer = CdbWriter::create("file.cdb");
//   if (writer) {
//       writer->add(key, keyLen, value, valueLen);
//       writer->finish();  // Must call to finalize
//   }
//=============================================================================

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace yetty {

//=============================================================================
// CdbReader - Read-only access to CDB files
//=============================================================================
class CdbReader {
public:
    using Ptr = std::unique_ptr<CdbReader>;

    virtual ~CdbReader() = default;

    // Open a CDB file for reading. Returns nullptr on failure.
    static Ptr open(const std::string& path);

    // Look up a key. Returns the value data, or nullopt if not found.
    virtual std::optional<std::vector<uint8_t>> get(const void* key, size_t keyLen) = 0;

    // Convenience overload for 4-byte keys (e.g., codepoints)
    std::optional<std::vector<uint8_t>> get(uint32_t key) {
        return get(&key, sizeof(key));
    }
};

//=============================================================================
// CdbWriter - Create new CDB files
//=============================================================================
class CdbWriter {
public:
    using Ptr = std::unique_ptr<CdbWriter>;

    virtual ~CdbWriter() = default;

    // Create a new CDB file for writing. Returns nullptr on failure.
    static Ptr create(const std::string& path);

    // Add a key-value pair. Returns true on success.
    virtual bool add(const void* key, size_t keyLen, const void* value, size_t valueLen) = 0;

    // Convenience overload for 4-byte keys
    bool add(uint32_t key, const void* value, size_t valueLen) {
        return add(&key, sizeof(key), value, valueLen);
    }

    // Finalize the CDB file. Must be called before destruction.
    // Returns true on success.
    virtual bool finish() = 0;
};

} // namespace yetty
