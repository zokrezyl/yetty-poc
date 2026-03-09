// DisplayList to YDraw converter
#pragma once

#include "displaylist-json.h"
#include "ydraw-buffer-standalone.h"
#include <vector>

namespace displaylist {

// State for transform/clip stack
struct RenderState {
    int translate_x = 0;
    int translate_y = 0;
    Rect clip_rect = {0, 0, 100000, 100000};  // Effectively infinite
};

// Convert a DisplayList to YDraw primitives
class DisplayListToYDraw {
public:
    DisplayListToYDraw(ydraw::Buffer& buffer);

    void execute(const DisplayList& list);

private:
    void executeCommand(const Command& cmd);

    // Command handlers
    void handleFillRect(const FillRect& cmd);
    void handleFillRectWithRoundedCorners(const FillRectWithRoundedCorners& cmd);
    void handleDrawGlyphRun(const DrawGlyphRun& cmd);
    void handleDrawLine(const DrawLine& cmd);
    void handleFillEllipse(const FillEllipse& cmd);
    void handleDrawEllipse(const DrawEllipse& cmd);
    void handleFillPath(const FillPath& cmd);
    void handleStrokePath(const StrokePath& cmd);
    void handlePaintLinearGradient(const PaintLinearGradient& cmd);
    void handlePaintRadialGradient(const PaintRadialGradient& cmd);
    void handleDrawScaledBitmap(const DrawScaledBitmap& cmd);
    void handlePaintOuterBoxShadow(const PaintOuterBoxShadow& cmd);
    void handlePaintInnerBoxShadow(const PaintInnerBoxShadow& cmd);
    void handleSave(const Save& cmd);
    void handleRestore(const Restore& cmd);
    void handleTranslate(const Translate& cmd);
    void handleAddClipRect(const AddClipRect& cmd);
    void handleAddRoundedRectClip(const AddRoundedRectClip& cmd);

    // Apply current transform to a point
    Point transform(const Point& p) const {
        return {p.x + _state.translate_x, p.y + _state.translate_y};
    }

    Point transform(int x, int y) const {
        return {x + _state.translate_x, y + _state.translate_y};
    }

    // Check if rect is visible after clipping
    bool isVisible(const Rect& r) const;

    // Intersect rect with clip rect
    Rect clip(const Rect& r) const;

    ydraw::Buffer& _buffer;
    RenderState _state;
    std::vector<RenderState> _stateStack;
};

} // namespace displaylist
