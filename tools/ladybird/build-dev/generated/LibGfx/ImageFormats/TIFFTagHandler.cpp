/*
 * Copyright (c) 2023, Lucas Chollet <lucas.chollet@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Debug.h>
#include <AK/String.h>
#include <LibGfx/ImageFormats/TIFFMetadata.h>

namespace Gfx::TIFF {

static String value_formatter(u32 tag_id, Value const& v) {
    switch (tag_id) {
        case 259:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<Compression>(v.get<u32>()))));
        case 262:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<PhotometricInterpretation>(v.get<u32>()))));
        case 266:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<FillOrder>(v.get<u32>()))));
        case 274:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<Orientation>(v.get<u32>()))));
        case 284:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<PlanarConfiguration>(v.get<u32>()))));
        case 296:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<ResolutionUnit>(v.get<u32>()))));
        case 317:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<Predictor>(v.get<u32>()))));
        case 338:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<ExtraSample>(v.get<u32>()))));
        case 339:
            return MUST(String::from_utf8(
                name_for_enum_tag_value(static_cast<SampleFormat>(v.get<u32>()))));
        default:
            return MUST(String::formatted("{}", v));
    }
}

[[maybe_unused]] static String format_tiff_value(u32 tag_id, Vector<Value> const& values) {
    if (values.size() == 1)
        return MUST(String::formatted("{}", value_formatter(tag_id, values[0])));

    StringBuilder builder;
    builder.append('[');

    for (u32 i = 0; i < values.size(); ++i) {
        builder.appendff("{}", value_formatter(tag_id, values[i]));
        if (i != values.size() - 1)
            builder.append(", "sv);
    }

    builder.append(']');
    return MUST(builder.to_string());
}

ErrorOr<void> ensure_baseline_tags_are_present(ExifMetadata const& metadata)
{
    if (!metadata.image_width().has_value())
        return Error::from_string_literal("Unable to decode image, missing required tag ImageWidth.");

    if (!metadata.image_length().has_value())
        return Error::from_string_literal("Unable to decode image, missing required tag ImageLength.");

    if (!metadata.compression().has_value())
        return Error::from_string_literal("Unable to decode image, missing required tag Compression.");

    if (!metadata.photometric_interpretation().has_value())
        return Error::from_string_literal("Unable to decode image, missing required tag PhotometricInterpretation.");

    return {};
}

ErrorOr<Type> tiff_type_from_u16(u16 type)
{
    switch (type) {
    case to_underlying(Type::Byte):
        return Type::Byte;
    case to_underlying(Type::ASCII):
        return Type::ASCII;
    case to_underlying(Type::UnsignedShort):
        return Type::UnsignedShort;
    case to_underlying(Type::UnsignedLong):
        return Type::UnsignedLong;
    case to_underlying(Type::UnsignedRational):
        return Type::UnsignedRational;
    case to_underlying(Type::Undefined):
        return Type::Undefined;
    case to_underlying(Type::SignedLong):
        return Type::SignedLong;
    case to_underlying(Type::SignedRational):
        return Type::SignedRational;
    case to_underlying(Type::Float):
        return Type::Float;
    case to_underlying(Type::Double):
        return Type::Double;
    case to_underlying(Type::IFD):
        return Type::IFD;
    case to_underlying(Type::UTF8):
        return Type::UTF8;
    default:
        return Error::from_string_literal("TIFFImageDecoderPlugin: Unknown type");
    }
}

u8 size_of_type(Type type)
{
    switch (type) {
    case Type::Byte:
        return 1;
    case Type::ASCII:
        return 1;
    case Type::UnsignedShort:
        return 2;
    case Type::UnsignedLong:
        return 4;
    case Type::UnsignedRational:
        return 8;
    case Type::Undefined:
        return 1;
    case Type::SignedLong:
        return 4;
    case Type::SignedRational:
        return 8;
    case Type::Float:
        return 4;
    case Type::Double:
        return 8;
    case Type::IFD:
        return 4;
    case Type::UTF8:
        return 1;
    default:
        VERIFY_NOT_REACHED();
    }
}

ErrorOr<void> handle_tag(Function<ErrorOr<void>(u32)>&& subifd_handler, ExifMetadata& metadata, u16 tag, Type type, u32 count, Vector<Value>&& value)
{
    switch (tag) {
    case 256:
        // ImageWidth

        dbgln_if(TIFF_DEBUG, "ImageWidth({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ImageWidth invalid");
        
        
        metadata.add_entry("ImageWidth"sv, move(value));
        break;

    case 257:
        // ImageLength

        dbgln_if(TIFF_DEBUG, "ImageLength({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ImageLength invalid");
        
        
        metadata.add_entry("ImageLength"sv, move(value));
        break;

    case 258:
        // BitsPerSample

        dbgln_if(TIFF_DEBUG, "BitsPerSample({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag BitsPerSample invalid");
        
        
        metadata.add_entry("BitsPerSample"sv, move(value));
        break;

    case 259:
        // Compression

        dbgln_if(TIFF_DEBUG, "Compression({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Compression invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2 && v != 3 && v != 4 && v != 5 && v != 6 && v != 7 && v != 8 && v != 32773 && v != 32946))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag Compression");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("Compression"sv, move(value));
        break;

    case 262:
        // PhotometricInterpretation

        dbgln_if(TIFF_DEBUG, "PhotometricInterpretation({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag PhotometricInterpretation invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 0 && v != 1 && v != 2 && v != 3 && v != 4 && v != 5 && v != 6 && v != 8))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag PhotometricInterpretation");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("PhotometricInterpretation"sv, move(value));
        break;

    case 266:
        // FillOrder

        dbgln_if(TIFF_DEBUG, "FillOrder({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag FillOrder invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag FillOrder");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("FillOrder"sv, move(value));
        break;

    case 271:
        // Make

        dbgln_if(TIFF_DEBUG, "Make({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Make invalid");
        
        
        metadata.add_entry("Make"sv, move(value));
        break;

    case 272:
        // Model

        dbgln_if(TIFF_DEBUG, "Model({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Model invalid");
        
        
        metadata.add_entry("Model"sv, move(value));
        break;

    case 273:
        // StripOffsets

        dbgln_if(TIFF_DEBUG, "StripOffsets({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag StripOffsets invalid");
        
        
        metadata.add_entry("StripOffsets"sv, move(value));
        break;

    case 274:
        // Orientation

        dbgln_if(TIFF_DEBUG, "Orientation({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Orientation invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2 && v != 3 && v != 4 && v != 5 && v != 6 && v != 7 && v != 8))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag Orientation");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("Orientation"sv, move(value));
        break;

    case 277:
        // SamplesPerPixel

        dbgln_if(TIFF_DEBUG, "SamplesPerPixel({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag SamplesPerPixel invalid");
        
        
        metadata.add_entry("SamplesPerPixel"sv, move(value));
        break;

    case 278:
        // RowsPerStrip

        dbgln_if(TIFF_DEBUG, "RowsPerStrip({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag RowsPerStrip invalid");
        
        
        metadata.add_entry("RowsPerStrip"sv, move(value));
        break;

    case 279:
        // StripByteCounts

        dbgln_if(TIFF_DEBUG, "StripByteCounts({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag StripByteCounts invalid");
        
        
        metadata.add_entry("StripByteCounts"sv, move(value));
        break;

    case 282:
        // XResolution

        dbgln_if(TIFF_DEBUG, "XResolution({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedRational)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag XResolution invalid");
        
        
        metadata.add_entry("XResolution"sv, move(value));
        break;

    case 283:
        // YResolution

        dbgln_if(TIFF_DEBUG, "YResolution({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedRational)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag YResolution invalid");
        
        
        metadata.add_entry("YResolution"sv, move(value));
        break;

    case 284:
        // PlanarConfiguration

        dbgln_if(TIFF_DEBUG, "PlanarConfiguration({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag PlanarConfiguration invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag PlanarConfiguration");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("PlanarConfiguration"sv, move(value));
        break;

    case 285:
        // PageName

        dbgln_if(TIFF_DEBUG, "PageName({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag PageName invalid");
        
        
        metadata.add_entry("PageName"sv, move(value));
        break;

    case 292:
        // T4Options

        dbgln_if(TIFF_DEBUG, "T4Options({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag T4Options invalid");
        
        
        metadata.add_entry("T4Options"sv, move(value));
        break;

    case 296:
        // ResolutionUnit

        dbgln_if(TIFF_DEBUG, "ResolutionUnit({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ResolutionUnit invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2 && v != 3))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag ResolutionUnit");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("ResolutionUnit"sv, move(value));
        break;

    case 305:
        // Software

        dbgln_if(TIFF_DEBUG, "Software({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Software invalid");
        
        
        metadata.add_entry("Software"sv, move(value));
        break;

    case 306:
        // DateTime

        dbgln_if(TIFF_DEBUG, "DateTime({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            || (count != 20))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag DateTime invalid");
        
        
        metadata.add_entry("DateTime"sv, move(value));
        break;

    case 315:
        // Artist

        dbgln_if(TIFF_DEBUG, "Artist({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::ASCII)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Artist invalid");
        
        
        metadata.add_entry("Artist"sv, move(value));
        break;

    case 317:
        // Predictor

        dbgln_if(TIFF_DEBUG, "Predictor({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag Predictor invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag Predictor");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("Predictor"sv, move(value));
        break;

    case 320:
        // ColorMap

        dbgln_if(TIFF_DEBUG, "ColorMap({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ColorMap invalid");
        
        
        metadata.add_entry("ColorMap"sv, move(value));
        break;

    case 322:
        // TileWidth

        dbgln_if(TIFF_DEBUG, "TileWidth({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag TileWidth invalid");
        
        
        metadata.add_entry("TileWidth"sv, move(value));
        break;

    case 323:
        // TileLength

        dbgln_if(TIFF_DEBUG, "TileLength({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag TileLength invalid");
        
        
        metadata.add_entry("TileLength"sv, move(value));
        break;

    case 324:
        // TileOffsets

        dbgln_if(TIFF_DEBUG, "TileOffsets({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag TileOffsets invalid");
        
        
        metadata.add_entry("TileOffsets"sv, move(value));
        break;

    case 325:
        // TileByteCounts

        dbgln_if(TIFF_DEBUG, "TileByteCounts({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort && type != Type::UnsignedLong)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag TileByteCounts invalid");
        
        
        metadata.add_entry("TileByteCounts"sv, move(value));
        break;

    case 338:
        // ExtraSamples

        dbgln_if(TIFF_DEBUG, "ExtraSamples({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ExtraSamples invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 0 && v != 1 && v != 2))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag ExtraSamples");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("ExtraSamples"sv, move(value));
        break;

    case 339:
        // SampleFormat

        dbgln_if(TIFF_DEBUG, "SampleFormat({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedShort)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag SampleFormat invalid");
        
        for (u32 i = 0; i < value.size(); ++i) {
            TRY(value[i].visit(
                [](u32 const& v) -> ErrorOr<void> {
                    if ((v != 1 && v != 2 && v != 3 && v != 4))
                        return Error::from_string_literal("TIFFImageDecoderPlugin: Invalid value for tag SampleFormat");
                    return {};
                },
                [&](auto const&) -> ErrorOr<void> {
                    VERIFY_NOT_REACHED();
                })
            );
        }

        
        metadata.add_entry("SampleFormat"sv, move(value));
        break;

    case 34665:
        // ExifIFD

        dbgln_if(TIFF_DEBUG, "ExifIFD({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::UnsignedLong && type != Type::IFD)
            || (count != 1))
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ExifIFD invalid");
        
        TRY(subifd_handler(value[0].get<u32>()));
        metadata.add_entry("ExifIFD"sv, move(value));
        break;

    case 34675:
        // ICCProfile

        dbgln_if(TIFF_DEBUG, "ICCProfile({}): {}", name_for_enum_tag_value(type), format_tiff_value(tag, value));

        if ((type != Type::Undefined)
            )
            return Error::from_string_literal("TIFFImageDecoderPlugin: Tag ICCProfile invalid");
        
        
        metadata.add_entry("ICCProfile"sv, move(value));
        break;

    default:
        dbgln_if(TIFF_DEBUG, "UnknownTag({}, {}): {}",
                tag, name_for_enum_tag_value(type), format_tiff_value(tag, value));
    }

    return {};
}

}
