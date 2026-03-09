/*
 * Copyright (c) 2023, Lucas Chollet <lucas.chollet@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/HashMap.h>
#include <AK/Variant.h>
#include <AK/Vector.h>
#include <LibGfx/Size.h>
#include <LibGfx/ImageFormats/ImageDecoder.h>

namespace Gfx {

class ExifMetadata;

namespace TIFF {

enum class Type {
    Byte = 1,
    ASCII = 2,
    UnsignedShort = 3,
    UnsignedLong = 4,
    UnsignedRational = 5,
    Undefined = 7,
    SignedLong = 9,
    SignedRational = 10,
    Float = 11,
    Double = 12,
    IFD = 13,
    UTF8 = 129,
};


template<OneOf<u32, i32> x32>
struct Rational {
    using Type = x32;
    x32 numerator;
    x32 denominator;

    double as_double() const {
        return static_cast<double>(numerator) / denominator;
    }
};

template<typename T>
struct TypePromoter {
    using Type = T;
};
template<>
struct TypePromoter<u16> {
    using Type = u32;
};
template<>
struct TypePromoter<float> {
    using Type = double;
};


// Note that u16 is not include on purpose
using Value = Variant<ByteBuffer, String, u32, Rational<u32>, i32, Rational<i32>, double>;

enum class Compression {
    NoCompression = 1,
    CCITTRLE = 2,
    Group3Fax = 3,
    Group4Fax = 4,
    LZW = 5,
    OldJPEG = 6,
    JPEG = 7,
    AdobeDeflate = 8,
    PackBits = 32773,
    PixarDeflate = 32946,
};

enum class PhotometricInterpretation {
    WhiteIsZero = 0,
    BlackIsZero = 1,
    RGB = 2,
    RGBPalette = 3,
    TransparencyMask = 4,
    CMYK = 5,
    YCbCr = 6,
    CIELab = 8,
};

enum class FillOrder {
    LeftToRight = 1,
    RightToLeft = 2,
};

enum class Orientation {
    Default = 1,
    FlipHorizontally = 2,
    Rotate180 = 3,
    FlipVertically = 4,
    Rotate90ClockwiseThenFlipHorizontally = 5,
    Rotate90Clockwise = 6,
    FlipHorizontallyThenRotate90Clockwise = 7,
    Rotate90CounterClockwise = 8,
};

enum class PlanarConfiguration {
    Chunky = 1,
    Planar = 2,
};

enum class ResolutionUnit {
    NoAbsolute = 1,
    Inch = 2,
    Centimeter = 3,
};

enum class Predictor {
    NoPrediction = 1,
    HorizontalDifferencing = 2,
};

enum class ExtraSample {
    Unspecified = 0,
    AssociatedAlpha = 1,
    UnassociatedAlpha = 2,
};

enum class SampleFormat {
    Unsigned = 1,
    Signed = 2,
    Float = 3,
    Undefined = 4,
};


template<Enum E>
StringView name_for_enum_tag_value(E value) {
    if constexpr (IsSame<E, Compression>) {
        switch (value) {
            default:
                return "Invalid value for Compression"sv;
            case Compression::NoCompression:
                return "NoCompression"sv;
            case Compression::CCITTRLE:
                return "CCITTRLE"sv;
            case Compression::Group3Fax:
                return "Group3Fax"sv;
            case Compression::Group4Fax:
                return "Group4Fax"sv;
            case Compression::LZW:
                return "LZW"sv;
            case Compression::OldJPEG:
                return "OldJPEG"sv;
            case Compression::JPEG:
                return "JPEG"sv;
            case Compression::AdobeDeflate:
                return "AdobeDeflate"sv;
            case Compression::PackBits:
                return "PackBits"sv;
            case Compression::PixarDeflate:
                return "PixarDeflate"sv;
        }
    }
    if constexpr (IsSame<E, PhotometricInterpretation>) {
        switch (value) {
            default:
                return "Invalid value for PhotometricInterpretation"sv;
            case PhotometricInterpretation::WhiteIsZero:
                return "WhiteIsZero"sv;
            case PhotometricInterpretation::BlackIsZero:
                return "BlackIsZero"sv;
            case PhotometricInterpretation::RGB:
                return "RGB"sv;
            case PhotometricInterpretation::RGBPalette:
                return "RGBPalette"sv;
            case PhotometricInterpretation::TransparencyMask:
                return "TransparencyMask"sv;
            case PhotometricInterpretation::CMYK:
                return "CMYK"sv;
            case PhotometricInterpretation::YCbCr:
                return "YCbCr"sv;
            case PhotometricInterpretation::CIELab:
                return "CIELab"sv;
        }
    }
    if constexpr (IsSame<E, FillOrder>) {
        switch (value) {
            default:
                return "Invalid value for FillOrder"sv;
            case FillOrder::LeftToRight:
                return "LeftToRight"sv;
            case FillOrder::RightToLeft:
                return "RightToLeft"sv;
        }
    }
    if constexpr (IsSame<E, Orientation>) {
        switch (value) {
            default:
                return "Invalid value for Orientation"sv;
            case Orientation::Default:
                return "Default"sv;
            case Orientation::FlipHorizontally:
                return "FlipHorizontally"sv;
            case Orientation::Rotate180:
                return "Rotate180"sv;
            case Orientation::FlipVertically:
                return "FlipVertically"sv;
            case Orientation::Rotate90ClockwiseThenFlipHorizontally:
                return "Rotate90ClockwiseThenFlipHorizontally"sv;
            case Orientation::Rotate90Clockwise:
                return "Rotate90Clockwise"sv;
            case Orientation::FlipHorizontallyThenRotate90Clockwise:
                return "FlipHorizontallyThenRotate90Clockwise"sv;
            case Orientation::Rotate90CounterClockwise:
                return "Rotate90CounterClockwise"sv;
        }
    }
    if constexpr (IsSame<E, PlanarConfiguration>) {
        switch (value) {
            default:
                return "Invalid value for PlanarConfiguration"sv;
            case PlanarConfiguration::Chunky:
                return "Chunky"sv;
            case PlanarConfiguration::Planar:
                return "Planar"sv;
        }
    }
    if constexpr (IsSame<E, ResolutionUnit>) {
        switch (value) {
            default:
                return "Invalid value for ResolutionUnit"sv;
            case ResolutionUnit::NoAbsolute:
                return "NoAbsolute"sv;
            case ResolutionUnit::Inch:
                return "Inch"sv;
            case ResolutionUnit::Centimeter:
                return "Centimeter"sv;
        }
    }
    if constexpr (IsSame<E, Predictor>) {
        switch (value) {
            default:
                return "Invalid value for Predictor"sv;
            case Predictor::NoPrediction:
                return "NoPrediction"sv;
            case Predictor::HorizontalDifferencing:
                return "HorizontalDifferencing"sv;
        }
    }
    if constexpr (IsSame<E, ExtraSample>) {
        switch (value) {
            default:
                return "Invalid value for ExtraSample"sv;
            case ExtraSample::Unspecified:
                return "Unspecified"sv;
            case ExtraSample::AssociatedAlpha:
                return "AssociatedAlpha"sv;
            case ExtraSample::UnassociatedAlpha:
                return "UnassociatedAlpha"sv;
        }
    }
    if constexpr (IsSame<E, SampleFormat>) {
        switch (value) {
            default:
                return "Invalid value for SampleFormat"sv;
            case SampleFormat::Unsigned:
                return "Unsigned"sv;
            case SampleFormat::Signed:
                return "Signed"sv;
            case SampleFormat::Float:
                return "Float"sv;
            case SampleFormat::Undefined:
                return "Undefined"sv;
        }
    }
    if constexpr (IsSame<E, Type>) {
        switch (value) {
            default:
                return "Invalid value for Type"sv;
            case Type::Byte:
                return "Byte"sv;
            case Type::ASCII:
                return "ASCII"sv;
            case Type::UnsignedShort:
                return "UnsignedShort"sv;
            case Type::UnsignedLong:
                return "UnsignedLong"sv;
            case Type::UnsignedRational:
                return "UnsignedRational"sv;
            case Type::Undefined:
                return "Undefined"sv;
            case Type::SignedLong:
                return "SignedLong"sv;
            case Type::SignedRational:
                return "SignedRational"sv;
            case Type::Float:
                return "Float"sv;
            case Type::Double:
                return "Double"sv;
            case Type::IFD:
                return "IFD"sv;
            case Type::UTF8:
                return "UTF8"sv;
        }
    }
    VERIFY_NOT_REACHED();
}

ErrorOr<void> handle_tag(Function<ErrorOr<void>(u32)>&& subifd_handler, ExifMetadata& metadata, u16 tag, Type type, u32 count, Vector<Value>&& value);
ErrorOr<void> ensure_baseline_tags_are_present(ExifMetadata const& metadata);
ErrorOr<Type> tiff_type_from_u16(u16 type);
u8 size_of_type(Type type);

}

class ExifMetadata : public Metadata {
public:
    virtual ~ExifMetadata() = default;

    Optional<u32> image_width() const
    {
        auto const& possible_value = m_data.get("ImageWidth"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<u32> image_length() const
    {
        auto const& possible_value = m_data.get("ImageLength"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<Vector<u32>> bits_per_sample() const
    {
        auto const& possible_value = m_data.get("BitsPerSample"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<TIFF::Compression> compression() const
    {
        auto const& possible_value = m_data.get("Compression"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return static_cast<TIFF::Compression>((*possible_value)[0].get<u32>());
    }

    Optional<TIFF::PhotometricInterpretation> photometric_interpretation() const
    {
        auto const& possible_value = m_data.get("PhotometricInterpretation"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return static_cast<TIFF::PhotometricInterpretation>((*possible_value)[0].get<u32>());
    }

    Optional<TIFF::FillOrder> fill_order() const
    {
        auto const& possible_value = m_data.get("FillOrder"sv);
        if (!possible_value.has_value())
            return TIFF::FillOrder::LeftToRight;
        return static_cast<TIFF::FillOrder>((*possible_value)[0].get<u32>());
    }

    Optional<String const&> make() const
    {
        auto const& possible_value = m_data.get("Make"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<String const&> model() const
    {
        auto const& possible_value = m_data.get("Model"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<Vector<u32>> strip_offsets() const
    {
        auto const& possible_value = m_data.get("StripOffsets"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<TIFF::Orientation> orientation() const
    {
        auto const& possible_value = m_data.get("Orientation"sv);
        if (!possible_value.has_value())
            return TIFF::Orientation::Default;
        return static_cast<TIFF::Orientation>((*possible_value)[0].get<u32>());
    }

    Optional<u32> samples_per_pixel() const
    {
        auto const& possible_value = m_data.get("SamplesPerPixel"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<u32> rows_per_strip() const
    {
        auto const& possible_value = m_data.get("RowsPerStrip"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<Vector<u32>> strip_byte_counts() const
    {
        auto const& possible_value = m_data.get("StripByteCounts"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<TIFF::Rational<u32>> x_resolution() const
    {
        auto const& possible_value = m_data.get("XResolution"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<TIFF::Rational<u32>>();
    }

    Optional<TIFF::Rational<u32>> y_resolution() const
    {
        auto const& possible_value = m_data.get("YResolution"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<TIFF::Rational<u32>>();
    }

    Optional<TIFF::PlanarConfiguration> planar_configuration() const
    {
        auto const& possible_value = m_data.get("PlanarConfiguration"sv);
        if (!possible_value.has_value())
            return TIFF::PlanarConfiguration::Chunky;
        return static_cast<TIFF::PlanarConfiguration>((*possible_value)[0].get<u32>());
    }

    Optional<String const&> page_name() const
    {
        auto const& possible_value = m_data.get("PageName"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<u32> t4_options() const
    {
        auto const& possible_value = m_data.get("T4Options"sv);
        if (!possible_value.has_value())
            return 0;
        return (*possible_value)[0].get<u32>();
    }

    Optional<TIFF::ResolutionUnit> resolution_unit() const
    {
        auto const& possible_value = m_data.get("ResolutionUnit"sv);
        if (!possible_value.has_value())
            return TIFF::ResolutionUnit::Inch;
        return static_cast<TIFF::ResolutionUnit>((*possible_value)[0].get<u32>());
    }

    Optional<String const&> software() const
    {
        auto const& possible_value = m_data.get("Software"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<String const&> date_time() const
    {
        auto const& possible_value = m_data.get("DateTime"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<String const&> artist() const
    {
        auto const& possible_value = m_data.get("Artist"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<String>();
    }

    Optional<TIFF::Predictor> predictor() const
    {
        auto const& possible_value = m_data.get("Predictor"sv);
        if (!possible_value.has_value())
            return TIFF::Predictor::NoPrediction;
        return static_cast<TIFF::Predictor>((*possible_value)[0].get<u32>());
    }

    Optional<Vector<u32>> color_map() const
    {
        auto const& possible_value = m_data.get("ColorMap"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<u32> tile_width() const
    {
        auto const& possible_value = m_data.get("TileWidth"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<u32> tile_length() const
    {
        auto const& possible_value = m_data.get("TileLength"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<Vector<u32>> tile_offsets() const
    {
        auto const& possible_value = m_data.get("TileOffsets"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<Vector<u32>> tile_byte_counts() const
    {
        auto const& possible_value = m_data.get("TileByteCounts"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<u32> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = (*possible_value)[i].get<u32>();

        return tmp;
    }

    Optional<Vector<TIFF::ExtraSample>> extra_samples() const
    {
        auto const& possible_value = m_data.get("ExtraSamples"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<TIFF::ExtraSample> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = static_cast<TIFF::ExtraSample>((*possible_value)[i].get<u32>());

        return tmp;
    }

    Optional<Vector<TIFF::SampleFormat>> sample_format() const
    {
        auto const& possible_value = m_data.get("SampleFormat"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        
        Vector<TIFF::SampleFormat> tmp{};
        auto maybe_failure = tmp.try_resize(possible_value->size());
        if (maybe_failure.is_error())
            return OptionalNone {};
        
        for (u32 i = 0; i < possible_value->size(); ++i)
            tmp[i] = static_cast<TIFF::SampleFormat>((*possible_value)[i].get<u32>());

        return tmp;
    }

    Optional<u32> exif_ifd() const
    {
        auto const& possible_value = m_data.get("ExifIFD"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<u32>();
    }

    Optional<ByteBuffer const&> icc_profile() const
    {
        auto const& possible_value = m_data.get("ICCProfile"sv);
        if (!possible_value.has_value())
            return OptionalNone {};
        return (*possible_value)[0].get<ByteBuffer>();
    }

private:
    friend ErrorOr<void> TIFF::handle_tag(Function<ErrorOr<void>(u32)>&& subifd_handler, ExifMetadata& metadata, u16 tag, TIFF::Type type, u32 count, Vector<TIFF::Value>&& value);

    virtual void fill_main_tags() const override {
        if (model().has_value())
            m_main_tags.set("Model"sv, model().value());
        if (make().has_value())
            m_main_tags.set("Manufacturer"sv, make().value());
        if (software().has_value())
            m_main_tags.set("Software"sv, software().value());
        if (date_time().has_value())
            m_main_tags.set("Creation Time"sv, date_time().value());
        if (artist().has_value())
            m_main_tags.set("Author"sv, artist().value());
    }

    void add_entry(StringView key, Vector<TIFF::Value>&& value) {
        m_data.set(key, move(value));
    }

    HashMap<StringView, Vector<TIFF::Value>> m_data;
};


}

template<typename T>
struct AK::Formatter<Gfx::TIFF::Rational<T>> : Formatter<FormatString> {
    ErrorOr<void> format(FormatBuilder& builder, Gfx::TIFF::Rational<T> value)
    {
        return Formatter<FormatString>::format(builder, "{} ({}/{})"sv,
            value.as_double(), value.numerator, value.denominator);
    }
};

template<>
struct AK::Formatter<Gfx::TIFF::Value> : Formatter<FormatString> {
    ErrorOr<void> format(FormatBuilder& builder, Gfx::TIFF::Value const& value)
    {
        String content;
        value.visit(
            [&](ByteBuffer const& buffer) {
                content = MUST(String::formatted("Buffer of size: {}", buffer.size()));
            },
            [&](auto const& other) {
                content = MUST(String::formatted("{}", other));
            }
        );

        return Formatter<FormatString>::format(builder, "{}"sv, content);
    }
};
