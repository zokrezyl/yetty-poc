#pragma once

#include <yetty/base/object.h>
#include <yetty/base/factory.h>
#include <yetty/result.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace yetty::font {

/// RawFont - lightweight FreeType-based font for text measurement.
/// No GPU dependencies - usable by remote clients (PDF rendering, etc.).
class RawFont : public base::Object,
                public base::ObjectFactory<RawFont> {
public:
    using Ptr = std::shared_ptr<RawFont>;

    ~RawFont() override = default;
    const char* typeName() const override { return "RawFont"; }

    /// Create from raw TTF/OTF data.
    static Result<Ptr> createImpl(const uint8_t* data, size_t size, const std::string& name);

    /// Font name.
    virtual const std::string& name() const = 0;

    /// Text measurement.
    virtual float measureTextWidth(const std::string& text, float fontSize) = 0;
    virtual float fontAscent(float fontSize) = 0;
    virtual float fontDescent(float fontSize) = 0;

protected:
    RawFont() = default;
};

} // namespace yetty::font
