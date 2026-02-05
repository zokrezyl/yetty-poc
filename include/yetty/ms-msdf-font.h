#pragma once

#include <yetty/font.h>
#include <yetty/msdf-atlas.h>
#include <yetty/base/base.h>

#include <string>
#include <memory>

namespace yetty {

//=============================================================================
// MsMsdfFont - multi-style MSDF font for the terminal grid
//
// Composes MsdfAtlas for atlas/CDB/GPU management.
// Implements Font interface with style dispatch (Regular/Bold/Italic/BoldItalic).
//
// Header = interface, implementation in ms-msdf-font.cpp (MsMsdfFontImpl).
//=============================================================================
class MsMsdfFont : public Font,
                   public base::Object,
                   public base::ObjectFactory<MsMsdfFont> {
public:
    using Ptr = std::shared_ptr<MsMsdfFont>;
    using base::ObjectFactory<MsMsdfFont>::create;

    static Result<Ptr> createImpl(ContextType& ctx,
                                   const std::string& cdbBasePath);

    ~MsMsdfFont() override = default;

    // Atlas access (for GPU resources, font registration, etc.)
    virtual MsdfAtlas::Ptr atlas() const = 0;

    // Set fallback CDB for CJK characters (loaded lazily on demand)
    virtual void setFallbackCdb(const std::string& cdbPath) = 0;
};

} // namespace yetty
