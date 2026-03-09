// DisplayListPlayerYDraw: Renders LibWeb DisplayList to YDraw buffer

#include "DisplayListPlayerYDraw.h"
#include <cstdio>

namespace ydraw {

DisplayListPlayerYDraw::DisplayListPlayerYDraw(Buffer& buffer)
    : _buffer(buffer)
{
    _current_state.translation = { 0, 0 };
    _current_state.clip_rect = { 0, 0, 10000, 10000 }; // Large default clip
}

void DisplayListPlayerYDraw::execute_to_ydraw(Web::Painting::DisplayList& display_list)
{
    // Iterate through commands and dispatch to virtual methods
    for (auto const& item : display_list.commands()) {
        auto const& command = item.command;

        command.visit(
            [this](Web::Painting::DrawGlyphRun const& cmd) { draw_glyph_run(cmd); },
            [this](Web::Painting::FillRect const& cmd) { fill_rect(cmd); },
            [this](Web::Painting::DrawScaledImmutableBitmap const& cmd) { draw_scaled_immutable_bitmap(cmd); },
            [this](Web::Painting::DrawRepeatedImmutableBitmap const& cmd) { draw_repeated_immutable_bitmap(cmd); },
            [this](Web::Painting::DrawExternalContent const& cmd) { draw_external_content(cmd); },
            [this](Web::Painting::Save const& cmd) { save(cmd); },
            [this](Web::Painting::SaveLayer const& cmd) { save_layer(cmd); },
            [this](Web::Painting::Restore const& cmd) { restore(cmd); },
            [this](Web::Painting::Translate const& cmd) { translate(cmd); },
            [this](Web::Painting::AddClipRect const& cmd) { add_clip_rect(cmd); },
            [this](Web::Painting::PaintLinearGradient const& cmd) { paint_linear_gradient(cmd); },
            [this](Web::Painting::PaintRadialGradient const& cmd) { paint_radial_gradient(cmd); },
            [this](Web::Painting::PaintConicGradient const& cmd) { paint_conic_gradient(cmd); },
            [this](Web::Painting::PaintOuterBoxShadow const& cmd) { paint_outer_box_shadow(cmd); },
            [this](Web::Painting::PaintInnerBoxShadow const& cmd) { paint_inner_box_shadow(cmd); },
            [this](Web::Painting::PaintTextShadow const& cmd) { paint_text_shadow(cmd); },
            [this](Web::Painting::FillRectWithRoundedCorners const& cmd) { fill_rect_with_rounded_corners(cmd); },
            [this](Web::Painting::FillPath const& cmd) { fill_path(cmd); },
            [this](Web::Painting::StrokePath const& cmd) { stroke_path(cmd); },
            [this](Web::Painting::DrawEllipse const& cmd) { draw_ellipse(cmd); },
            [this](Web::Painting::FillEllipse const& cmd) { fill_ellipse(cmd); },
            [this](Web::Painting::DrawLine const& cmd) { draw_line(cmd); },
            [this](Web::Painting::ApplyBackdropFilter const& cmd) { apply_backdrop_filter(cmd); },
            [this](Web::Painting::DrawRect const& cmd) { draw_rect(cmd); },
            [this](Web::Painting::AddRoundedRectClip const& cmd) { add_rounded_rect_clip(cmd); },
            [this](Web::Painting::PaintNestedDisplayList const& cmd) { paint_nested_display_list(cmd); },
            [this](Web::Painting::PaintScrollBar const& cmd) { paint_scrollbar(cmd); },
            [this](Web::Painting::ApplyEffects const& cmd) { apply_effects(cmd); }
        );
    }
}

void DisplayListPlayerYDraw::flush()
{
    // Nothing to do - YDraw buffer doesn't need flushing
}

void DisplayListPlayerYDraw::draw_glyph_run(Web::Painting::DrawGlyphRun const& cmd)
{
    // Extract text from glyph run
    // For now, use the rect position and render placeholder text
    auto& rect = cmd.rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());

    // Try to get font size from the glyph run
    float fontSize = 16.0f; // Default
    // GlyphRun is NonnullRefPtr - always valid
    // fontSize = cmd.glyph_run->font().point_size();

    // Get text from glyph run - this is simplified
    // Real implementation would need to extract Unicode text from glyphs
    _buffer.addText("[text]", x, y, fontSize, toColor(cmd.color));
}

void DisplayListPlayerYDraw::fill_rect(Web::Painting::FillRect const& cmd)
{
    auto& rect = cmd.rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    _buffer.addBox(x, y, w, h, toColor(cmd.color));
}

void DisplayListPlayerYDraw::draw_scaled_immutable_bitmap(Web::Painting::DrawScaledImmutableBitmap const& cmd)
{
    // Draw placeholder rectangle for images
    auto& rect = cmd.dst_rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    // Gray placeholder for images
    _buffer.addBox(x, y, w, h, packColor(200, 200, 200, 255));
}

void DisplayListPlayerYDraw::draw_repeated_immutable_bitmap(Web::Painting::DrawRepeatedImmutableBitmap const& cmd)
{
    // Same as above - placeholder
    auto& rect = cmd.dst_rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    _buffer.addBox(x, y, w, h, packColor(180, 180, 180, 255));
}

void DisplayListPlayerYDraw::draw_external_content(Web::Painting::DrawExternalContent const& cmd)
{
    // External content (iframes, plugins) - draw placeholder
    auto& rect = cmd.dst_rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    _buffer.addBox(x, y, w, h, packColor(220, 220, 240, 255));
}

void DisplayListPlayerYDraw::save(Web::Painting::Save const&)
{
    _state_stack.push_back(_current_state);
}

void DisplayListPlayerYDraw::save_layer(Web::Painting::SaveLayer const&)
{
    _state_stack.push_back(_current_state);
}

void DisplayListPlayerYDraw::restore(Web::Painting::Restore const&)
{
    if (!_state_stack.empty()) {
        _current_state = _state_stack.back();
        _state_stack.pop_back();
    }
}

void DisplayListPlayerYDraw::translate(Web::Painting::Translate const& cmd)
{
    _current_state.translation.set_x(_current_state.translation.x() + cmd.delta.x());
    _current_state.translation.set_y(_current_state.translation.y() + cmd.delta.y());
}

void DisplayListPlayerYDraw::add_clip_rect(Web::Painting::AddClipRect const& cmd)
{
    // Store clip rect (simplified - doesn't intersect with existing)
    _current_state.clip_rect = cmd.rect;
}

void DisplayListPlayerYDraw::paint_linear_gradient(Web::Painting::PaintLinearGradient const& cmd)
{
    // Simplified: use first color stop or average
    auto& rect = cmd.gradient_rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    // TODO: Extract color from gradient data
    _buffer.addBox(x, y, w, h, packColor(150, 150, 200, 255));
}

void DisplayListPlayerYDraw::paint_radial_gradient(Web::Painting::PaintRadialGradient const& cmd)
{
    auto& rect = cmd.rect;
    float cx = static_cast<float>(rect.x() + rect.width() / 2 + _current_state.translation.x());
    float cy = static_cast<float>(rect.y() + rect.height() / 2 + _current_state.translation.y());
    float r = static_cast<float>(std::min(rect.width(), rect.height()) / 2);

    _buffer.addCircle(cx, cy, r, packColor(150, 200, 150, 255));
}

void DisplayListPlayerYDraw::paint_conic_gradient(Web::Painting::PaintConicGradient const& cmd)
{
    // Approximate with circle
    auto& rect = cmd.rect;
    float cx = static_cast<float>(rect.x() + rect.width() / 2 + _current_state.translation.x());
    float cy = static_cast<float>(rect.y() + rect.height() / 2 + _current_state.translation.y());
    float r = static_cast<float>(std::min(rect.width(), rect.height()) / 2);

    _buffer.addCircle(cx, cy, r, packColor(200, 150, 150, 255));
}

void DisplayListPlayerYDraw::paint_outer_box_shadow(Web::Painting::PaintOuterBoxShadow const& cmd)
{
    // Draw shadow as larger semi-transparent rounded box
    auto const& params = cmd.box_shadow_params;
    float x = static_cast<float>(params.device_content_rect.x() + _current_state.translation.x() - params.spread_distance);
    float y = static_cast<float>(params.device_content_rect.y() + _current_state.translation.y() - params.spread_distance);
    float w = static_cast<float>(params.device_content_rect.width() + params.spread_distance * 2);
    float h = static_cast<float>(params.device_content_rect.height() + params.spread_distance * 2);

    // Semi-transparent shadow color
    auto color = params.color;
    _buffer.addRoundedBox(x + params.offset_x, y + params.offset_y, w, h,
                          static_cast<float>(params.corner_radii.top_left.horizontal_radius),
                          packColor(color.red(), color.green(), color.blue(), color.alpha() / 2));
}

void DisplayListPlayerYDraw::paint_inner_box_shadow(Web::Painting::PaintInnerBoxShadow const& cmd)
{
    // Inner shadow - approximate with darker border
    (void)cmd;
}

void DisplayListPlayerYDraw::paint_text_shadow(Web::Painting::PaintTextShadow const& cmd)
{
    // Text shadow - draw offset semi-transparent text
    float x = static_cast<float>(cmd.draw_location.x() + _current_state.translation.x());
    float y = static_cast<float>(cmd.draw_location.y() + _current_state.translation.y());

    auto color = cmd.color;
    _buffer.addText("[shadow]", x, y, 14.0f,
                    packColor(color.red(), color.green(), color.blue(), color.alpha() / 2));
}

void DisplayListPlayerYDraw::fill_rect_with_rounded_corners(Web::Painting::FillRectWithRoundedCorners const& cmd)
{
    auto& rect = cmd.rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    // Use average of corner radii
    float r = static_cast<float>(
        cmd.corner_radii.top_left.horizontal_radius +
        cmd.corner_radii.top_right.horizontal_radius +
        cmd.corner_radii.bottom_left.horizontal_radius +
        cmd.corner_radii.bottom_right.horizontal_radius
    ) / 4.0f;

    _buffer.addRoundedBox(x, y, w, h, r, toColor(cmd.color));
}

void DisplayListPlayerYDraw::fill_path(Web::Painting::FillPath const& cmd)
{
    // Approximate path with bounding box
    auto& rect = cmd.path_bounding_rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    // TODO: Decompose path segments
    Gfx::Color color;
    if (auto* c = cmd.paint_style_or_color.get_pointer<Gfx::Color>()) {
        color = *c;
    } else {
        color = Gfx::Color::Black;
    }
    _buffer.addBox(x, y, w, h, toColor(color));
}

void DisplayListPlayerYDraw::stroke_path(Web::Painting::StrokePath const& cmd)
{
    // TODO: Decompose path into line segments
    (void)cmd;
}

void DisplayListPlayerYDraw::draw_ellipse(Web::Painting::DrawEllipse const& cmd)
{
    auto& rect = cmd.rect;
    float cx = static_cast<float>(rect.x() + rect.width() / 2 + _current_state.translation.x());
    float cy = static_cast<float>(rect.y() + rect.height() / 2 + _current_state.translation.y());
    float r = static_cast<float>(std::min(rect.width(), rect.height()) / 2);

    // Stroke ellipse - approximate with circle (arc not available)
    // TODO: implement proper ellipse stroke
    (void)cx; (void)cy; (void)r; (void)cmd;
}

void DisplayListPlayerYDraw::fill_ellipse(Web::Painting::FillEllipse const& cmd)
{
    auto& rect = cmd.rect;
    float cx = static_cast<float>(rect.x() + rect.width() / 2 + _current_state.translation.x());
    float cy = static_cast<float>(rect.y() + rect.height() / 2 + _current_state.translation.y());
    float r = static_cast<float>(std::min(rect.width(), rect.height()) / 2);

    _buffer.addCircle(cx, cy, r, toColor(cmd.color));
}

void DisplayListPlayerYDraw::draw_line(Web::Painting::DrawLine const& cmd)
{
    float x1 = static_cast<float>(cmd.from.x() + _current_state.translation.x());
    float y1 = static_cast<float>(cmd.from.y() + _current_state.translation.y());
    float x2 = static_cast<float>(cmd.to.x() + _current_state.translation.x());
    float y2 = static_cast<float>(cmd.to.y() + _current_state.translation.y());

    _buffer.addSegment(x1, y1, x2, y2, static_cast<float>(cmd.thickness), toColor(cmd.color));
}

void DisplayListPlayerYDraw::apply_backdrop_filter(Web::Painting::ApplyBackdropFilter const& cmd)
{
    // Backdrop filters (blur, etc.) - not supported in YDraw
    (void)cmd;
}

void DisplayListPlayerYDraw::draw_rect(Web::Painting::DrawRect const& cmd)
{
    auto& rect = cmd.rect;
    float x = static_cast<float>(rect.x() + _current_state.translation.x());
    float y = static_cast<float>(rect.y() + _current_state.translation.y());
    float w = static_cast<float>(rect.width());
    float h = static_cast<float>(rect.height());

    // Draw rectangle outline
    float thickness = 1.0f;
    uint32_t color = toColor(cmd.color);

    // Top
    _buffer.addSegment(x, y, x + w, y, thickness, color);
    // Right
    _buffer.addSegment(x + w, y, x + w, y + h, thickness, color);
    // Bottom
    _buffer.addSegment(x, y + h, x + w, y + h, thickness, color);
    // Left
    _buffer.addSegment(x, y, x, y + h, thickness, color);
}

void DisplayListPlayerYDraw::add_rounded_rect_clip(Web::Painting::AddRoundedRectClip const& cmd)
{
    _current_state.clip_rect = cmd.border_rect;
}

void DisplayListPlayerYDraw::paint_nested_display_list(Web::Painting::PaintNestedDisplayList const& cmd)
{
    if (cmd.display_list) {
        execute_to_ydraw(*cmd.display_list);
    }
}

void DisplayListPlayerYDraw::paint_scrollbar(Web::Painting::PaintScrollBar const& cmd)
{
    // Draw scrollbar track
    auto& gutter = cmd.gutter_rect;
    float gx = static_cast<float>(gutter.x() + _current_state.translation.x());
    float gy = static_cast<float>(gutter.y() + _current_state.translation.y());
    float gw = static_cast<float>(gutter.width());
    float gh = static_cast<float>(gutter.height());
    _buffer.addBox(gx, gy, gw, gh, toColor(cmd.track_color));

    // Draw scrollbar thumb
    auto& thumb = cmd.thumb_rect;
    float tx = static_cast<float>(thumb.x() + _current_state.translation.x());
    float ty = static_cast<float>(thumb.y() + _current_state.translation.y());
    float tw = static_cast<float>(thumb.width());
    float th = static_cast<float>(thumb.height());
    _buffer.addRoundedBox(tx, ty, tw, th, 4.0f, toColor(cmd.thumb_color));
}

void DisplayListPlayerYDraw::apply_effects(Web::Painting::ApplyEffects const& cmd)
{
    // Effects like opacity, blend modes - limited support
    (void)cmd;
}

void DisplayListPlayerYDraw::apply_transform(Gfx::FloatPoint origin, Gfx::FloatMatrix4x4 const& matrix)
{
    // 3D transforms - approximate with 2D translation
    (void)origin;
    (void)matrix;
}

bool DisplayListPlayerYDraw::would_be_fully_clipped_by_painter(Gfx::IntRect rect) const
{
    // Check if rect is outside current clip
    return !_current_state.clip_rect.intersects(rect);
}

void DisplayListPlayerYDraw::add_clip_path(Gfx::Path const& path)
{
    // Path clipping - approximate with bounding box
    auto bbox = path.bounding_box();
    _current_state.clip_rect = bbox.to_rounded<int>();
}

} // namespace ydraw
