#pragma once

#include "../../ydraw/ydraw-base.h"
#include <yetty/base/factory.h>
#include <string>

// Forward declarations for YAML
namespace YAML { class Node; }

namespace yetty::card {

//=============================================================================
// YDraw - SDF card with YAML/binary payload parsing
//
// Subclasses YDrawBase to add YAML and binary format parsing.
// All rendering is handled by the base class.
//=============================================================================
class YDraw : public yetty::YDrawBase,
              public base::ObjectFactory<YDraw> {
public:
    using Ptr = std::shared_ptr<YDraw>;

    //=========================================================================
    // Factory methods
    //=========================================================================
    static Result<CardPtr> create(
        const YettyContext& ctx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload);

    static Result<Ptr> createImpl(
        ContextType& ctx,
        const YettyContext& yettyCtx,
        int32_t x, int32_t y,
        uint32_t widthCells, uint32_t heightCells,
        const std::string& args,
        const std::string& payload) noexcept;

    ~YDraw() override = default;

    const char* typeName() const override { return "ydraw"; }

    // Constructor is public for make_shared, use create() factory instead
    YDraw(const YettyContext& ctx,
          int32_t x, int32_t y,
          uint32_t widthCells, uint32_t heightCells,
          const std::string& args, const std::string& payload);

    Result<void> init();

    // Payload parsing
    void parseArgs(const std::string& args);
    Result<void> parsePayload(const std::string& payload);
    Result<void> parseBinary(const std::string& payload);
    Result<void> parseYAML(const std::string& yaml);
    void parseYAMLPrimitive(const YAML::Node& item);
    static uint32_t parseColor(const YAML::Node& node);

    std::string _argsStr;
    std::string _payloadStr;
};

} // namespace yetty::card
