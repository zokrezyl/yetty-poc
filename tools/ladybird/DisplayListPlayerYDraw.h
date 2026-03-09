// DisplayListPlayerYDraw: Renders LibWeb DisplayList to YDraw buffer
#pragma once

#include "ydraw-buffer-standalone.h"
#include <LibGfx/PaintingSurface.h>
#include <LibWeb/Painting/DisplayList.h>
#include <LibWeb/Painting/DisplayListCommand.h>
#include <vector>

namespace ydraw {

class DisplayListPlayerYDraw final : public Web::Painting::DisplayListPlayer {
public:
    explicit DisplayListPlayerYDraw(Buffer& buffer);
    ~DisplayListPlayerYDraw() override = default;

    // Execute display list without requiring PaintingSurface
    void execute_to_ydraw(Web::Painting::DisplayList& display_list);

private:
    // DisplayListPlayer interface
    void flush() override;
    void draw_glyph_run(Web::Painting::DrawGlyphRun const&) override;
    void fill_rect(Web::Painting::FillRect const&) override;
    void draw_scaled_immutable_bitmap(Web::Painting::DrawScaledImmutableBitmap const&) override;
    void draw_repeated_immutable_bitmap(Web::Painting::DrawRepeatedImmutableBitmap const&) override;
    void draw_external_content(Web::Painting::DrawExternalContent const&) override;
    void save(Web::Painting::Save const&) override;
    void save_layer(Web::Painting::SaveLayer const&) override;
    void restore(Web::Painting::Restore const&) override;
    void translate(Web::Painting::Translate const&) override;
    void add_clip_rect(Web::Painting::AddClipRect const&) override;
    void paint_linear_gradient(Web::Painting::PaintLinearGradient const&) override;
    void paint_radial_gradient(Web::Painting::PaintRadialGradient const&) override;
    void paint_conic_gradient(Web::Painting::PaintConicGradient const&) override;
    void paint_outer_box_shadow(Web::Painting::PaintOuterBoxShadow const&) override;
    void paint_inner_box_shadow(Web::Painting::PaintInnerBoxShadow const&) override;
    void paint_text_shadow(Web::Painting::PaintTextShadow const&) override;
    void fill_rect_with_rounded_corners(Web::Painting::FillRectWithRoundedCorners const&) override;
    void fill_path(Web::Painting::FillPath const&) override;
    void stroke_path(Web::Painting::StrokePath const&) override;
    void draw_ellipse(Web::Painting::DrawEllipse const&) override;
    void fill_ellipse(Web::Painting::FillEllipse const&) override;
    void draw_line(Web::Painting::DrawLine const&) override;
    void apply_backdrop_filter(Web::Painting::ApplyBackdropFilter const&) override;
    void draw_rect(Web::Painting::DrawRect const&) override;
    void add_rounded_rect_clip(Web::Painting::AddRoundedRectClip const&) override;
    void paint_nested_display_list(Web::Painting::PaintNestedDisplayList const&) override;
    void paint_scrollbar(Web::Painting::PaintScrollBar const&) override;
    void apply_effects(Web::Painting::ApplyEffects const&) override;
    void apply_transform(Gfx::FloatPoint origin, Gfx::FloatMatrix4x4 const&) override;
    bool would_be_fully_clipped_by_painter(Gfx::IntRect) const override;
    void add_clip_path(Gfx::Path const&) override;

    // Helper to convert LibGfx color to YDraw color
    static uint32_t toColor(Gfx::Color c) {
        return packColor(c.red(), c.green(), c.blue(), c.alpha());
    }

    Buffer& _buffer;

    // State stack for save/restore
    struct State {
        Gfx::IntPoint translation;
        Gfx::IntRect clip_rect;
    };
    std::vector<State> _state_stack;
    State _current_state;
};

} // namespace ydraw
