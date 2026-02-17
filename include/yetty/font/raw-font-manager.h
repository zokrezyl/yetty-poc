#pragma once

#include <yetty/font/raw-font.h>
#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace yetty::font {

/// RawFontManager - thread singleton for creating RawFont instances.
/// Manages the thread-local FreeType library internally.
class RawFontManager : public base::Object,
                       public base::ThreadSingleton<RawFontManager> {
public:
    using Ptr = std::shared_ptr<RawFontManager>;

    ~RawFontManager() override = default;
    const char* typeName() const override { return "RawFontManager"; }

    static Result<Ptr> createImpl();

    /// Create a RawFont from raw TTF/OTF data.
    virtual Result<RawFont::Ptr> createFromData(const uint8_t* data, size_t size,
                                                 const std::string& name) = 0;

    /// Create a RawFont from a font file path.
    virtual Result<RawFont::Ptr> createFromFile(const std::string& path) = 0;

protected:
    RawFontManager() = default;
};

} // namespace yetty::font
