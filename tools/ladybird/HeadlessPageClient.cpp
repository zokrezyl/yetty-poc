// HeadlessPageClient implementation
#include "HeadlessPageClient.h"

namespace ydraw {

GC_DEFINE_ALLOCATOR(HeadlessPageClient);

HeadlessPageClient::HeadlessPageClient(int width, int height)
    : m_width(width)
    , m_height(height)
{
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
