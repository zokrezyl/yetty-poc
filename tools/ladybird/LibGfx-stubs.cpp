// LibGfx stubs for no-Skia build
// Provides minimal implementations of Skia-dependent classes

#include <AK/Error.h>
#include <AK/NonnullOwnPtr.h>
#include <AK/OwnPtr.h>
#include <LibGfx/Bitmap.h>
#include <LibGfx/Color.h>
#include <LibGfx/ColorSpace.h>
#include <LibGfx/Filter.h>
#include <LibGfx/Font/Font.h>
#include <LibGfx/ImmutableBitmap.h>
#include <LibGfx/PaintingSurface.h>
#include <LibGfx/ShareableBitmap.h>
#include <LibGfx/TextLayout.h>
#include <LibIPC/Decoder.h>
#include <LibIPC/Encoder.h>

// Forward declarations for Skia types (in global namespace)
class SkImage;
class SkCanvas;
class SkSurface;
class SkTextBlob;

namespace Gfx {

// === FilterImpl stub ===
struct FilterImpl {
    NonnullOwnPtr<FilterImpl> clone() const { return make<FilterImpl>(); }
};

Filter::Filter(Filter const& other)
    : m_impl(other.m_impl->clone())
{
}

Filter& Filter::operator=(Filter const& other)
{
    if (this != &other) {
        m_impl = other.m_impl->clone();
    }
    return *this;
}

Filter::~Filter() = default;

Filter::Filter(NonnullOwnPtr<FilterImpl>&& impl)
    : m_impl(move(impl))
{
}

FilterImpl const& Filter::impl() const
{
    return *m_impl;
}

// Static factory methods - return stub filters using placement new
Filter Filter::arithmetic(Optional<Filter const&>, Optional<Filter const&>, float, float, float, float)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::compose(Filter const&, Filter const&)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::blend(Optional<Filter const&>, Optional<Filter const&>, CompositingAndBlendingOperator)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::flood(Gfx::Color, float)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::displacement_map(Optional<Filter const&>, Optional<Filter const&>, float, ChannelSelector, ChannelSelector)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::drop_shadow(float, float, float, Gfx::Color, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::blur(float, float, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::color(ColorFilterType, float, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::color_matrix(float[20], Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::color_table(Optional<ReadonlyBytes>, Optional<ReadonlyBytes>, Optional<ReadonlyBytes>, Optional<ReadonlyBytes>, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::saturate(float, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::hue_rotate(float, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::image(Gfx::ImmutableBitmap const&, Gfx::IntRect const&, Gfx::IntRect const&, Gfx::ScalingMode)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::merge(Vector<Optional<Filter>> const&)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::offset(float, float, Optional<Filter const&>)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::erode(float, float, Optional<Filter> const&)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::dilate(float, float, Optional<Filter> const&)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

Filter Filter::turbulence(TurbulenceType, float, float, i32, float, Gfx::IntSize const&)
{
    alignas(Filter) static char storage[sizeof(Filter)];
    auto* f = new (storage) Filter(make<FilterImpl>());
    return *f;
}

// === Bitmap stubs ===
struct BackingStore {
    void* data { nullptr };
    size_t pitch { 0 };
    size_t size_in_bytes { 0 };
};

StringView bitmap_format_name(BitmapFormat format)
{
    switch (format) {
#define ENUMERATE_BITMAP_FORMAT(f) case BitmapFormat::f: return #f##sv;
    ENUMERATE_BITMAP_FORMATS(ENUMERATE_BITMAP_FORMAT)
#undef ENUMERATE_BITMAP_FORMAT
    }
    VERIFY_NOT_REACHED();
}

size_t Bitmap::minimum_pitch(size_t width, BitmapFormat format)
{
    size_t element_size = 4; // All formats are 4 bytes per pixel
    (void)format;
    return width * element_size;
}

static bool size_would_overflow(BitmapFormat format, IntSize size)
{
    if (size.width() < 0 || size.height() < 0)
        return true;
    if (size.width() >= INT16_MAX || size.height() >= INT16_MAX)
        return true;
    size_t pitch = Bitmap::minimum_pitch(size.width(), format);
    return Checked<size_t>::multiplication_would_overflow(pitch, size.height());
}

ErrorOr<BackingStore> Bitmap::allocate_backing_store(BitmapFormat format, IntSize size, InitializeBackingStore initialize)
{
    if (size.is_empty())
        return Error::from_string_literal("Gfx::Bitmap backing store size is empty");
    if (size_would_overflow(format, size))
        return Error::from_string_literal("Gfx::Bitmap backing store size overflow");
    auto const pitch = Bitmap::minimum_pitch(size.width(), format);
    auto const data_size = pitch * size.height();
    void* data;
    if (initialize == InitializeBackingStore::Yes)
        data = calloc(1, data_size);
    else
        data = malloc(data_size);
    if (!data)
        return Error::from_errno(ENOMEM);
    return BackingStore { data, pitch, data_size };
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create(BitmapFormat format, IntSize size)
{
    return create(format, AlphaType::Premultiplied, size);
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create(BitmapFormat format, AlphaType alpha_type, IntSize size)
{
    auto backing_store = TRY(Bitmap::allocate_backing_store(format, size));
    return adopt_nonnull_ref_or_enomem(new (nothrow) Bitmap(format, alpha_type, size, backing_store));
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create_shareable(BitmapFormat, AlphaType, IntSize)
{
    return Error::from_string_literal("Bitmap::create_shareable not implemented in no-Skia build");
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create_wrapper(BitmapFormat format, AlphaType alpha_type, IntSize size, size_t pitch, void* data, Function<void()>&& destruction_callback)
{
    if (size_would_overflow(format, size))
        return Error::from_string_literal("Gfx::Bitmap::create_wrapper size overflow");
    return adopt_ref(*new Bitmap(format, alpha_type, size, pitch, data, move(destruction_callback)));
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create_with_anonymous_buffer(BitmapFormat format, AlphaType alpha_type, Core::AnonymousBuffer buffer, IntSize size)
{
    if (size_would_overflow(format, size))
        return Error::from_string_literal("Gfx::Bitmap::create_with_anonymous_buffer size overflow");
    return adopt_nonnull_ref_or_enomem(new (nothrow) Bitmap(format, alpha_type, move(buffer), size));
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::create_with_raw_data(BitmapFormat format, AlphaType alpha_type, ReadonlyBytes raw_data, IntSize size)
{
    if (size_would_overflow(format, size))
        return Error::from_string_literal("Gfx::Bitmap::create_with_raw_data size overflow");
    auto backing_store = TRY(Bitmap::allocate_backing_store(format, size, InitializeBackingStore::No));
    raw_data.copy_to(Bytes { backing_store.data, backing_store.size_in_bytes });
    return adopt_nonnull_ref_or_enomem(new (nothrow) Bitmap(format, alpha_type, size, backing_store));
}

Bitmap::Bitmap(BitmapFormat format, AlphaType alpha_type, IntSize size, BackingStore const& backing_store)
    : m_size(size)
    , m_data(backing_store.data)
    , m_pitch(backing_store.pitch)
    , m_format(format)
    , m_alpha_type(alpha_type)
{
    m_destruction_callback = [data = m_data, size_in_bytes = backing_store.size_in_bytes] {
        free(data);
        (void)size_in_bytes;
    };
}

Bitmap::Bitmap(BitmapFormat format, AlphaType alpha_type, IntSize size, size_t pitch, void* data, Function<void()>&& destruction_callback)
    : m_size(size)
    , m_data(data)
    , m_pitch(pitch)
    , m_format(format)
    , m_alpha_type(alpha_type)
    , m_destruction_callback(move(destruction_callback))
{
}

Bitmap::Bitmap(BitmapFormat format, AlphaType alpha_type, Core::AnonymousBuffer buffer, IntSize size)
    : m_size(size)
    , m_data(buffer.data<void>())
    , m_pitch(minimum_pitch(size.width(), format))
    , m_format(format)
    , m_alpha_type(alpha_type)
    , m_buffer(move(buffer))
{
}

Bitmap::~Bitmap()
{
    if (m_destruction_callback)
        m_destruction_callback();
    m_data = nullptr;
}

ErrorOr<NonnullRefPtr<Gfx::Bitmap>> Bitmap::clone() const
{
    auto new_bitmap = TRY(Bitmap::create(format(), alpha_type(), size()));
    memcpy(new_bitmap->scanline(0), scanline(0), size_in_bytes());
    return new_bitmap;
}

ErrorOr<NonnullRefPtr<Gfx::Bitmap>> Bitmap::cropped(Gfx::IntRect crop, Gfx::Color outside_color) const
{
    if (crop == rect())
        return clone();
    auto new_bitmap = TRY(Gfx::Bitmap::create(format(), alpha_type(), { crop.width(), crop.height() }));
    for (int y = 0; y < crop.height(); ++y) {
        for (int x = 0; x < crop.width(); ++x) {
            int global_x = x + crop.left();
            int global_y = y + crop.top();
            if (global_x >= width() || global_y >= height() || global_x < 0 || global_y < 0)
                new_bitmap->set_pixel(x, y, outside_color);
            else
                new_bitmap->set_pixel(x, y, get_pixel(global_x, global_y));
        }
    }
    return new_bitmap;
}

ErrorOr<NonnullRefPtr<Bitmap>> Bitmap::scaled(int, int, ScalingMode) const
{
    return Error::from_string_literal("Bitmap::scaled not implemented in no-Skia build");
}

ErrorOr<NonnullRefPtr<Gfx::Bitmap>> Bitmap::to_bitmap_backed_by_anonymous_buffer() const
{
    if (m_buffer.is_valid())
        return NonnullRefPtr { const_cast<Bitmap&>(*this) };
    return Error::from_string_literal("Bitmap::to_bitmap_backed_by_anonymous_buffer not implemented in no-Skia build");
}

ShareableBitmap Bitmap::to_shareable_bitmap() const
{
    return {};
}

void Bitmap::strip_alpha_channel()
{
    for (auto& pixel : *this)
        pixel = 0xff000000 | (pixel & 0xffffff);
    m_format = BitmapFormat::BGRx8888;
}

Bitmap::DiffResult Bitmap::diff(Bitmap const& other) const
{
    DiffResult result;
    result.identical = true;
    if (size() != other.size()) {
        result.identical = false;
    }
    return result;
}

void Bitmap::set_alpha_type_destructive(AlphaType alpha_type)
{
    m_alpha_type = alpha_type;
}

// === ColorSpace stubs ===
namespace Details {
struct ColorSpaceImpl {
    // Empty - no Skia color space
};
}

ColorSpace::ColorSpace()
    : m_color_space(make<Details::ColorSpaceImpl>())
{
}

ColorSpace::ColorSpace(ColorSpace const&)
    : m_color_space(make<Details::ColorSpaceImpl>())
{
}

ColorSpace& ColorSpace::operator=(ColorSpace const&)
{
    m_color_space = make<Details::ColorSpaceImpl>();
    return *this;
}

ColorSpace::ColorSpace(ColorSpace&&) = default;
ColorSpace& ColorSpace::operator=(ColorSpace&&) = default;
ColorSpace::~ColorSpace() = default;

ColorSpace::ColorSpace(NonnullOwnPtr<Details::ColorSpaceImpl>&& color_space)
    : m_color_space(move(color_space))
{
}

ErrorOr<ColorSpace> ColorSpace::from_cicp(Media::CodingIndependentCodePoints)
{
    return ColorSpace {};
}

ErrorOr<ColorSpace> ColorSpace::load_from_icc_bytes(ReadonlyBytes)
{
    return ColorSpace {};
}

} // namespace Gfx

namespace IPC {

template<>
ErrorOr<void> encode(Encoder& encoder, Gfx::ColorSpace const&)
{
    TRY(encoder.encode<u64>(0));
    return {};
}

template<>
ErrorOr<Gfx::ColorSpace> decode(Decoder& decoder)
{
    auto size = TRY(decoder.decode<u64>());
    if (size != 0) {
        auto buffer = TRY(ByteBuffer::create_uninitialized(size));
        TRY(decoder.decode_into(buffer.bytes()));
    }
    return Gfx::ColorSpace {};
}

} // namespace IPC

namespace Gfx {

// === Font stubs ===
Font::~Font() = default;
Font::ShapingCache::~ShapingCache() = default;
void Font::ShapingCache::clear() {}

// === ImmutableBitmapImpl stub - NO Bitmap reference ===
struct ImmutableBitmapImpl {
    IntSize size;
    void* data { nullptr }; // Raw pixel data if any
};

int ImmutableBitmap::width() const { return m_impl->size.width(); }
int ImmutableBitmap::height() const { return m_impl->size.height(); }
IntRect ImmutableBitmap::rect() const { return { {}, size() }; }
IntSize ImmutableBitmap::size() const { return m_impl->size; }
AlphaType ImmutableBitmap::alpha_type() const { return AlphaType::Premultiplied; }

SkImage const* ImmutableBitmap::sk_image() const { return nullptr; }

ErrorOr<BitmapExportResult> ImmutableBitmap::export_to_byte_buffer(ExportFormat, int, Optional<int>, Optional<int>) const
{
    return Error::from_string_literal("ImmutableBitmap::export_to_byte_buffer not implemented in no-Skia build");
}

RefPtr<Bitmap const> ImmutableBitmap::bitmap() const { return nullptr; }
bool ImmutableBitmap::is_yuv_backed() const { return false; }

bool ImmutableBitmap::ensure_sk_image(SkiaBackendContext&) const { return false; }

Color ImmutableBitmap::get_pixel(int, int) const
{
    return Color::Black;
}

NonnullRefPtr<ImmutableBitmap> ImmutableBitmap::create(NonnullRefPtr<Bitmap> bitmap, ColorSpace)
{
    auto size = bitmap->size();
    ImmutableBitmapImpl impl {
        .size = size,
        .data = nullptr,
    };
    return adopt_ref(*new ImmutableBitmap(make<ImmutableBitmapImpl>(move(impl))));
}

NonnullRefPtr<ImmutableBitmap> ImmutableBitmap::create(NonnullRefPtr<Bitmap> bitmap, AlphaType, ColorSpace color_space)
{
    return create(move(bitmap), move(color_space));
}

NonnullRefPtr<ImmutableBitmap> ImmutableBitmap::create_snapshot_from_painting_surface(NonnullRefPtr<PaintingSurface>)
{
    VERIFY_NOT_REACHED(); // Not supported in no-Skia build
}

ErrorOr<NonnullRefPtr<ImmutableBitmap>> ImmutableBitmap::create_from_yuv(NonnullOwnPtr<YUVData>)
{
    return Error::from_string_literal("YUV not supported in no-Skia build");
}

ImmutableBitmap::ImmutableBitmap(NonnullOwnPtr<ImmutableBitmapImpl> impl)
    : m_impl(move(impl))
{
}

ImmutableBitmap::~ImmutableBitmap() = default;

void ImmutableBitmap::lock_context() {}
void ImmutableBitmap::unlock_context() {}

StringView export_format_name(ExportFormat format)
{
    switch (format) {
#define ENUMERATE_EXPORT_FORMAT(f) case ExportFormat::f: return #f##sv;
    ENUMERATE_EXPORT_FORMATS(ENUMERATE_EXPORT_FORMAT)
#undef ENUMERATE_EXPORT_FORMAT
    }
    VERIFY_NOT_REACHED();
}

// === PaintingSurface::Impl stub - NO Bitmap reference ===
struct PaintingSurface::Impl {
    IntSize size;
};

NonnullRefPtr<PaintingSurface> PaintingSurface::create_with_size(RefPtr<SkiaBackendContext>, IntSize size, BitmapFormat, AlphaType)
{
    return adopt_ref(*new PaintingSurface(make<Impl>(size)));
}

NonnullRefPtr<PaintingSurface> PaintingSurface::wrap_bitmap(Bitmap& bitmap)
{
    return adopt_ref(*new PaintingSurface(make<Impl>(bitmap.size())));
}

void PaintingSurface::read_into_bitmap(Bitmap&) {}
void PaintingSurface::write_from_bitmap(Bitmap const&) {}
void PaintingSurface::notify_content_will_change() {}

IntSize PaintingSurface::size() const { return m_impl->size; }
IntRect PaintingSurface::rect() const { return { {}, m_impl->size }; }

SkCanvas& PaintingSurface::canvas() const { VERIFY_NOT_REACHED(); }
SkSurface& PaintingSurface::sk_surface() const { VERIFY_NOT_REACHED(); }

RefPtr<SkiaBackendContext> PaintingSurface::skia_backend_context() const { return nullptr; }

void PaintingSurface::flush()
{
    if (on_flush)
        on_flush(*this);
}

PaintingSurface::PaintingSurface(NonnullOwnPtr<Impl>&& impl)
    : m_impl(move(impl))
{
}

PaintingSurface::~PaintingSurface() = default;

void PaintingSurface::lock_context() const {}
void PaintingSurface::unlock_context() const {}

// Explicit template instantiation
template<>
void* PaintingSurface::sk_image_snapshot() const { return nullptr; }

// === GlyphRun stubs - store font as raw pointer, don't ref ===
struct GlyphRun::CachedTextBlob {};

GlyphRun::GlyphRun(Vector<DrawGlyph>&& glyphs, NonnullRefPtr<Font const> font, TextType text_type, float width)
    : m_glyphs(move(glyphs))
    , m_font(move(font))
    , m_text_type(text_type)
    , m_width(width)
{
}

GlyphRun::~GlyphRun() = default;

NonnullRefPtr<GlyphRun> GlyphRun::slice(size_t start, size_t length) const
{
    Vector<DrawGlyph> sliced_glyphs;
    float width = 0;
    for (size_t i = start; i < start + length && i < m_glyphs.size(); ++i) {
        sliced_glyphs.append(m_glyphs[i]);
        width += m_glyphs[i].glyph_width;
    }
    return adopt_ref(*new GlyphRun(move(sliced_glyphs), m_font, m_text_type, width));
}

void GlyphRun::ensure_text_blob(float) const {}

FloatRect GlyphRun::cached_blob_bounds() const { return {}; }

SkTextBlob* GlyphRun::cached_skia_text_blob() const { return nullptr; }

} // namespace Gfx
