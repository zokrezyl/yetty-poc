
#pragma once

#include <AK/Optional.h>
#include <AK/Types.h>

namespace Web::HTML {

// Uses u32 to match the `first` field of NamedCharacterReferenceCodepoints for bit-field packing purposes.
enum class NamedCharacterReferenceSecondCodepoint : u32 {
    None,
    CombiningLongSolidusOverlay, // U+0338
    CombiningLongVerticalLineOverlay, // U+20D2
    HairSpace, // U+200A
    CombiningDoubleLowLine, // U+0333
    CombiningReverseSolidusOverlay, // U+20E5
    VariationSelector1, // U+FE00
    LatinSmallLetterJ, // U+006A
    CombiningMacronBelow, // U+0331
};

inline Optional<u16> named_character_reference_second_codepoint_value(NamedCharacterReferenceSecondCodepoint codepoint)
{
    switch (codepoint) {
    case NamedCharacterReferenceSecondCodepoint::None:
        return {};
    case NamedCharacterReferenceSecondCodepoint::CombiningLongSolidusOverlay:
        return 0x0338;
    case NamedCharacterReferenceSecondCodepoint::CombiningLongVerticalLineOverlay:
        return 0x20D2;
    case NamedCharacterReferenceSecondCodepoint::HairSpace:
        return 0x200A;
    case NamedCharacterReferenceSecondCodepoint::CombiningDoubleLowLine:
        return 0x0333;
    case NamedCharacterReferenceSecondCodepoint::CombiningReverseSolidusOverlay:
        return 0x20E5;
    case NamedCharacterReferenceSecondCodepoint::VariationSelector1:
        return 0xFE00;
    case NamedCharacterReferenceSecondCodepoint::LatinSmallLetterJ:
        return 0x006A;
    case NamedCharacterReferenceSecondCodepoint::CombiningMacronBelow:
        return 0x0331;
    default:
        VERIFY_NOT_REACHED();
    }
}

// Note: The first codepoint could fit in 17 bits, and the second could fit in 4 (if unsigned).
// However, to get any benefit from minimizing the struct size, it would need to be accompanied by
// bit-packing the g_named_character_reference_codepoints_lookup array.
struct NamedCharacterReferenceCodepoints {
    u32 first : 24; // Largest value is U+1D56B
    NamedCharacterReferenceSecondCodepoint second : 8;
};
static_assert(sizeof(NamedCharacterReferenceCodepoints) == 4);

struct NamedCharacterReferenceFirstLayerNode {
    // Really only needs 12 bits.
    u16 number;
};
static_assert(sizeof(NamedCharacterReferenceFirstLayerNode) == 2);

struct NamedCharacterReferenceFirstToSecondLayerLink {
    u64 mask : 52;
    u64 second_layer_offset : 12;
};
static_assert(sizeof(NamedCharacterReferenceFirstToSecondLayerLink) == 8);

// Note: It is possible to fit this information within 24 bits, which could then allow for tightly
// bit-packing the second layer array. This would reduce the size of the array by 630 bytes.
struct NamedCharacterReferenceSecondLayerNode {
    // Could be 10 bits
    u16 child_index;
    u8 number;
    // Could be 4 bits
    u8 children_len : 7;
    bool end_of_word : 1;
};
static_assert(sizeof(NamedCharacterReferenceSecondLayerNode) == 4);

struct NamedCharacterReferenceNode {
    // The actual alphabet of characters used in the list of named character references only
    // includes 61 unique characters ('1'...'8', ';', 'a'...'z', 'A'...'Z').
    u8 character;
    // Typically, nodes are numbered with "an integer which gives the number of words that
    // would be accepted by the automaton starting from that state." This numbering
    // allows calculating "a one-to-one correspondence between the integers 1 to L
    // (L is the number of words accepted by the automaton) and the words themselves."
    //
    // This allows us to have a minimal perfect hashing scheme such that it's possible to store
    // and lookup the codepoint transformations of each named character reference using a separate
    // array.
    //
    // This uses that idea, but instead of storing a per-node number that gets built up while
    // searching a list of children, the cumulative number that would result from adding together
    // the numbers of all the previous sibling nodes is stored instead. This cuts down on a bit
    // of work done while searching while keeping the minimal perfect hashing strategy intact.
    //
    // Empirically, the largest number in our DAFSA is 51, so all number values could fit in a u6.
    u8 number : 7;
    bool end_of_word : 1;
    // Index of the first child of this node.
    // There are 3190 nodes in our DAFSA after the first and second layers were extracted out, so
    // all indexes can fit in a u12 (there would be 3872 nodes with the first/second layers
    // included, so still a u12).
    u16 child_index : 12;
    u16 children_len : 4;
};
static_assert(sizeof(NamedCharacterReferenceNode) == 4);

extern NamedCharacterReferenceNode g_named_character_reference_nodes[];
extern NamedCharacterReferenceFirstLayerNode g_named_character_reference_first_layer[];
extern NamedCharacterReferenceFirstToSecondLayerLink g_named_character_reference_first_to_second_layer[];
extern NamedCharacterReferenceSecondLayerNode g_named_character_reference_second_layer[];

Optional<NamedCharacterReferenceCodepoints> named_character_reference_codepoints_from_unique_index(u16 unique_index);

} // namespace Web::HTML

