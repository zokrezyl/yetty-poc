#include <yetty/cdb-wrapper.h>
#include <ytrace/ytrace.hpp>

#include <cstdio>
#include <cstring>

#ifdef YETTY_USE_HOWERJ_CDB
//=============================================================================
// howerj/cdb implementation - Portable, works on all platforms
//=============================================================================
#include <cdb.h>

namespace yetty {

//-----------------------------------------------------------------------------
// File I/O callbacks for howerj/cdb
//-----------------------------------------------------------------------------
namespace {

static cdb_word_t file_read(void* file, void* buf, size_t length) {
    return static_cast<cdb_word_t>(fread(buf, 1, length, static_cast<FILE*>(file)));
}

static cdb_word_t file_write(void* file, void* buf, size_t length) {
    return static_cast<cdb_word_t>(fwrite(buf, 1, length, static_cast<FILE*>(file)));
}

static int file_seek(void* file, long offset) {
    return fseek(static_cast<FILE*>(file), offset, SEEK_SET);
}

static void* file_open(const char* name, int mode) {
    return fopen(name, mode == CDB_RW_MODE ? "wb+" : "rb");
}

static int file_close(void* file) {
    return fclose(static_cast<FILE*>(file));
}

static int file_flush(void* file) {
    return fflush(static_cast<FILE*>(file));
}

static cdb_options_t make_options() {
    cdb_options_t opts = {};
    opts.read = file_read;
    opts.write = file_write;
    opts.seek = file_seek;
    opts.open = file_open;
    opts.close = file_close;
    opts.flush = file_flush;
    return opts;
}

} // anonymous namespace

//-----------------------------------------------------------------------------
// CdbReaderImpl (howerj)
//-----------------------------------------------------------------------------
class CdbReaderImpl : public CdbReader {
public:
    ~CdbReaderImpl() override {
        if (_cdb) {
            cdb_close(_cdb);
            _cdb = nullptr;
        }
    }

    static Ptr open(const std::string& path) {
        auto reader = std::make_unique<CdbReaderImpl>();

        reader->_opts = make_options();
        int result = cdb_open(&reader->_cdb, &reader->_opts, CDB_RO_MODE, path.c_str());
        if (result < 0 || !reader->_cdb) {
            ywarn("CdbReader: Cannot open file: {}", path);
            return nullptr;
        }

        return reader;
    }

    std::optional<std::vector<uint8_t>> get(const void* key, size_t keyLen) override {
        cdb_buffer_t keyBuf = {};
        keyBuf.length = keyLen;
        keyBuf.buffer = const_cast<char*>(static_cast<const char*>(key));

        cdb_file_pos_t pos = {};

        int result = cdb_get(_cdb, &keyBuf, &pos);
        if (result <= 0) {
            return std::nullopt;  // Not found or error
        }

        // Seek to the value position and read it
        if (cdb_seek(_cdb, pos.position) < 0) {
            return std::nullopt;
        }

        std::vector<uint8_t> value(pos.length);
        if (cdb_read(_cdb, value.data(), pos.length) < 0) {
            return std::nullopt;
        }

        return value;
    }

private:
    cdb_options_t _opts = {};
    cdb_t* _cdb = nullptr;
};

//-----------------------------------------------------------------------------
// CdbWriterImpl (howerj)
//-----------------------------------------------------------------------------
class CdbWriterImpl : public CdbWriter {
public:
    ~CdbWriterImpl() override {
        if (_cdb) {
            cdb_close(_cdb);
            _cdb = nullptr;
        }
    }

    static Ptr create(const std::string& path) {
        auto writer = std::make_unique<CdbWriterImpl>();

        writer->_opts = make_options();
        int result = cdb_open(&writer->_cdb, &writer->_opts, CDB_RW_MODE, path.c_str());
        if (result < 0 || !writer->_cdb) {
            ywarn("CdbWriter: Cannot create file: {}", path);
            return nullptr;
        }

        return writer;
    }

    bool add(const void* key, size_t keyLen, const void* value, size_t valueLen) override {
        cdb_buffer_t keyBuf = {};
        keyBuf.length = keyLen;
        keyBuf.buffer = const_cast<char*>(static_cast<const char*>(key));

        cdb_buffer_t valBuf = {};
        valBuf.length = valueLen;
        valBuf.buffer = const_cast<char*>(static_cast<const char*>(value));

        return cdb_add(_cdb, &keyBuf, &valBuf) >= 0;
    }

    bool finish() override {
        if (!_cdb) return false;
        int result = cdb_close(_cdb);
        _cdb = nullptr;
        return result >= 0;
    }

private:
    cdb_options_t _opts = {};
    cdb_t* _cdb = nullptr;
};

// Factory functions
CdbReader::Ptr CdbReader::open(const std::string& path) {
    return CdbReaderImpl::open(path);
}

CdbWriter::Ptr CdbWriter::create(const std::string& path) {
    return CdbWriterImpl::create(path);
}

} // namespace yetty

#else // djb/cdb implementation
//=============================================================================
// djb/cdb implementation - Unix only (uses mmap)
//=============================================================================
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

extern "C" {
#include <cdb.h>
#include <cdb_make.h>
}

namespace yetty {

//-----------------------------------------------------------------------------
// CdbReaderImpl (djb)
//-----------------------------------------------------------------------------
class CdbReaderImpl : public CdbReader {
public:
    ~CdbReaderImpl() override {
        if (_cdb) {
            cdb_free(_cdb);
            delete _cdb;
        }
        if (_mapped && _mapped != MAP_FAILED) {
            munmap(_mapped, _size);
        }
        if (_fd >= 0) {
            close(_fd);
        }
    }

    static Ptr open(const std::string& path) {
        auto reader = std::make_unique<CdbReaderImpl>();

        reader->_fd = ::open(path.c_str(), O_RDONLY);
        if (reader->_fd < 0) {
            ywarn("CdbReader: Cannot open file: {}", path);
            return nullptr;
        }

        struct stat st;
        if (fstat(reader->_fd, &st) < 0) {
            ywarn("CdbReader: Cannot stat file: {}", path);
            return nullptr;
        }
        reader->_size = st.st_size;

        reader->_mapped = mmap(nullptr, reader->_size, PROT_READ, MAP_SHARED, reader->_fd, 0);
        if (reader->_mapped == MAP_FAILED) {
            ywarn("CdbReader: Cannot mmap file: {}", path);
            return nullptr;
        }

        reader->_cdb = new struct cdb;
        cdb_init(reader->_cdb, reader->_fd);

        return reader;
    }

    std::optional<std::vector<uint8_t>> get(const void* key, size_t keyLen) override {
        if (cdb_find(_cdb, static_cast<const char*>(key), keyLen) <= 0) {
            return std::nullopt;
        }

        unsigned int dataLen = cdb_datalen(_cdb);
        unsigned int dataPos = cdb_datapos(_cdb);

        std::vector<uint8_t> value(dataLen);
        if (cdb_read(_cdb, reinterpret_cast<char*>(value.data()), dataLen, dataPos) < 0) {
            return std::nullopt;
        }

        return value;
    }

private:
    int _fd = -1;
    void* _mapped = nullptr;
    size_t _size = 0;
    struct cdb* _cdb = nullptr;
};

//-----------------------------------------------------------------------------
// CdbWriterImpl (djb)
//-----------------------------------------------------------------------------
class CdbWriterImpl : public CdbWriter {
public:
    ~CdbWriterImpl() override {
        if (_fd >= 0) {
            close(_fd);
        }
    }

    static Ptr create(const std::string& path) {
        auto writer = std::make_unique<CdbWriterImpl>();

        writer->_fd = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (writer->_fd < 0) {
            ywarn("CdbWriter: Cannot create file: {}", path);
            return nullptr;
        }

        cdb_make_start(&writer->_cdbm, writer->_fd);
        writer->_started = true;

        return writer;
    }

    bool add(const void* key, size_t keyLen, const void* value, size_t valueLen) override {
        return cdb_make_add(&_cdbm, static_cast<const char*>(key), keyLen,
                           static_cast<const char*>(value), valueLen) >= 0;
    }

    bool finish() override {
        if (!_started) return false;
        int result = cdb_make_finish(&_cdbm);
        _started = false;
        if (_fd >= 0) {
            close(_fd);
            _fd = -1;
        }
        return result >= 0;
    }

private:
    int _fd = -1;
    struct cdb_make _cdbm = {};
    bool _started = false;
};

// Factory functions
CdbReader::Ptr CdbReader::open(const std::string& path) {
    return CdbReaderImpl::open(path);
}

CdbWriter::Ptr CdbWriter::create(const std::string& path) {
    return CdbWriterImpl::create(path);
}

} // namespace yetty

#endif // YETTY_USE_HOWERJ_CDB
