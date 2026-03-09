// HeadlessPageClient: Minimal PageClient for headless rendering
#pragma once

#include <LibGC/Ptr.h>
#include <LibGfx/Palette.h>
#include <LibGfx/Rect.h>
#include <LibWeb/Page/Page.h>
#include <LibWeb/Page/InputEvent.h>
#include <LibWeb/PixelUnits.h>
#include <queue>

namespace ydraw {

class HeadlessPageClient final : public Web::PageClient {
    GC_CELL(HeadlessPageClient, Web::PageClient);
    GC_DECLARE_ALLOCATOR(HeadlessPageClient);

public:
    static GC::Ref<HeadlessPageClient> create(JS::VM& vm, int width, int height);

    virtual Web::Page& page() override { return *m_page; }
    virtual Web::Page const& page() const override { return *m_page; }

    void set_page(GC::Ref<Web::Page> page) { m_page = page; }

    // Pure virtual methods that must be implemented
    virtual u64 id() const override { return 1; }
    virtual bool is_connection_open() const override { return true; }
    virtual Gfx::Palette palette() const override { return Gfx::Palette(*m_palette_impl); }
    virtual Web::DevicePixelRect screen_rect() const override { return { 0, 0, m_width, m_height }; }
    virtual double zoom_level() const override { return 1.0; }
    virtual double device_pixel_ratio() const override { return 1.0; }
    virtual double device_pixels_per_css_pixel() const override { return 1.0; }
    virtual Web::CSS::PreferredColorScheme preferred_color_scheme() const override { return Web::CSS::PreferredColorScheme::Light; }
    virtual Web::CSS::PreferredContrast preferred_contrast() const override { return Web::CSS::PreferredContrast::NoPreference; }
    virtual Web::CSS::PreferredMotion preferred_motion() const override { return Web::CSS::PreferredMotion::NoPreference; }
    virtual size_t screen_count() const override { return 1; }
    virtual Queue<Web::QueuedInputEvent>& input_event_queue() override { return m_input_queue; }
    virtual void report_finished_handling_input_event(u64, Web::EventResult) override {}
    virtual void request_file(Web::FileRequest) override {}

private:
    HeadlessPageClient(int width, int height);
    virtual void visit_edges(JS::Cell::Visitor&) override;

    GC::Ptr<Web::Page> m_page;
    RefPtr<Gfx::PaletteImpl> m_palette_impl;
    int m_width;
    int m_height;
    Queue<Web::QueuedInputEvent> m_input_queue;
};

} // namespace ydraw
