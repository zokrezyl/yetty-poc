// HeadlessPageClient implementation
#include "HeadlessPageClient.h"
#include <LibGfx/SystemTheme.h>

namespace ydraw {

GC_DEFINE_ALLOCATOR(HeadlessPageClient);

HeadlessPageClient::HeadlessPageClient(int width, int height)
    : m_width(width)
    , m_height(height)
{
    // Create a default palette
    auto buffer_or_error = Core::AnonymousBuffer::create_with_size(sizeof(Gfx::SystemTheme));
    if (!buffer_or_error.is_error()) {
        auto buffer = buffer_or_error.release_value();
        auto* theme = buffer.data<Gfx::SystemTheme>();
        // Set some reasonable defaults
        theme->color[to_underlying(Gfx::ColorRole::Window)] = Gfx::Color::White.value();
        theme->color[to_underlying(Gfx::ColorRole::WindowText)] = Gfx::Color::Black.value();
        theme->color[to_underlying(Gfx::ColorRole::Base)] = Gfx::Color::White.value();
        theme->color[to_underlying(Gfx::ColorRole::BaseText)] = Gfx::Color::Black.value();
        theme->color[to_underlying(Gfx::ColorRole::Button)] = Gfx::Color::LightGray.value();
        theme->color[to_underlying(Gfx::ColorRole::ButtonText)] = Gfx::Color::Black.value();
        m_palette_impl = Gfx::PaletteImpl::create_with_anonymous_buffer(buffer);
    }
}

GC::Ref<HeadlessPageClient> HeadlessPageClient::create(JS::VM& vm, int width, int height)
{
    return vm.heap().allocate<HeadlessPageClient>(width, height);
}

void HeadlessPageClient::visit_edges(JS::Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_page);
}

} // namespace ydraw
