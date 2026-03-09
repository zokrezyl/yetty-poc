// HeadlessPageClient: Minimal PageClient for headless rendering
#pragma once

#include <LibGC/Ptr.h>
#include <LibGfx/Rect.h>
#include <LibWeb/Page/Page.h>
#include <LibWeb/PixelUnits.h>

namespace ydraw {

class HeadlessPageClient final : public Web::PageClient {
    GC_CELL(HeadlessPageClient, Web::PageClient);

public:
    static GC::Ref<HeadlessPageClient> create(JS::VM& vm, int width, int height);

    virtual Web::Page& page() override { return *m_page; }
    virtual Web::Page const& page() const override { return *m_page; }

    void set_page(GC::Ref<Web::Page> page) { m_page = page; }

    // PageClient interface - minimal implementations
    virtual Web::DevicePixelRect screen_rect() const override { return { 0, 0, m_width, m_height }; }
    virtual Web::DevicePixelSize viewport_size() const override { return { m_width, m_height }; }
    virtual double device_pixels_per_css_pixel() const override { return 1.0; }
    virtual Web::CSS::PreferredColorScheme preferred_color_scheme() const override { return Web::CSS::PreferredColorScheme::Light; }
    virtual Web::CSS::PreferredContrast preferred_contrast() const override { return Web::CSS::PreferredContrast::NoPreference; }
    virtual Web::CSS::PreferredMotion preferred_motion() const override { return Web::CSS::PreferredMotion::NoPreference; }

    virtual void page_did_request_scroll_to(Web::CSSPixelPoint) override {}
    virtual void page_did_set_viewport_size(Web::CSSPixelSize) override {}
    virtual void page_did_layout() override { m_layout_done = true; }
    virtual void page_did_change_title(ByteString const&) override {}
    virtual void page_did_request_navigate_back() override {}
    virtual void page_did_request_navigate_forward() override {}
    virtual void page_did_request_refresh() override {}
    virtual void page_did_request_cursor_change(Gfx::Cursor const&) override {}
    virtual void page_did_enter_tooltip_area(Web::CSSPixelPoint, ByteString const&) override {}
    virtual void page_did_leave_tooltip_area() override {}
    virtual void page_did_hover_link(URL::URL const&) override {}
    virtual void page_did_unhover_link() override {}
    virtual void page_did_middle_click_link(URL::URL const&, ByteString const&, unsigned) override {}
    virtual void page_did_start_loading(URL::URL const&, bool) override {}
    virtual void page_did_create_new_document(Web::DOM::Document&) override {}
    virtual void page_did_finish_loading(URL::URL const&) override { m_load_done = true; }
    virtual void page_did_finish_text_test(String const&) override {}
    virtual void page_did_request_alert(String const&) override {}
    virtual void page_did_request_confirm(String const&) override {}
    virtual void page_did_request_prompt(String const&, String const&) override {}
    virtual void page_did_request_set_prompt_text(String const&) override {}
    virtual void page_did_request_accept_dialog() override {}
    virtual void page_did_request_dismiss_dialog() override {}
    virtual void page_did_change_favicon(Gfx::Bitmap const&) override {}
    virtual void page_did_request_image_context_menu(Web::CSSPixelPoint, URL::URL const&, ByteString const&, unsigned, Gfx::Bitmap const*) override {}
    virtual void page_did_request_link_context_menu(Web::CSSPixelPoint, URL::URL const&, ByteString const&, unsigned) override {}
    virtual void page_did_click_link(URL::URL const&, ByteString const&, unsigned) override {}
    virtual void page_did_request_context_menu(Web::CSSPixelPoint) override {}
    virtual void page_did_request_media_context_menu(Web::CSSPixelPoint, ByteString const&, unsigned, Web::Page::MediaContextMenu) override {}
    virtual void page_did_output_js_console_message(size_t) override {}
    virtual void page_did_change_audio_play_state(Web::HTML::AudioPlayState) override {}
    virtual Vector<Web::HTML::SelectItem> page_did_request_select_dropdown(Web::CSSPixelPoint, Web::CSSPixels, Vector<Web::HTML::SelectItem>) override { return {}; }
    virtual void page_did_request_dismiss_select_dropdown() override {}
    virtual void page_did_request_color_picker(Gfx::Color) override {}
    virtual void page_did_request_file_picker(Web::HTML::FileFilter, Web::HTML::AllowMultipleFiles) override {}
    virtual void page_did_request_text_selection(bool) override {}
    virtual void page_did_find_in_page(size_t, Optional<size_t>) override {}
    virtual void page_did_update_selection_state() override {}

    bool is_load_done() const { return m_load_done; }
    bool is_layout_done() const { return m_layout_done; }

private:
    HeadlessPageClient(int width, int height);
    virtual void visit_edges(JS::Cell::Visitor&) override;

    GC::Ptr<Web::Page> m_page;
    int m_width;
    int m_height;
    bool m_load_done = false;
    bool m_layout_done = false;
};

} // namespace ydraw
