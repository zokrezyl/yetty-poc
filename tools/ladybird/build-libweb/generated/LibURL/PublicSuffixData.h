#pragma once

#include <AK/Forward.h>
#include <AK/Trie.h>
#include <AK/Variant.h>

namespace URL {

class PublicSuffixData {
protected:
    PublicSuffixData();

public:
    PublicSuffixData(PublicSuffixData const&) = delete;
    PublicSuffixData& operator=(PublicSuffixData const&) = delete;

    static PublicSuffixData* the()
    {
        static PublicSuffixData* s_the;
        if (!s_the)
            s_the = new PublicSuffixData;
        return s_the;
    }

    bool is_public_suffix(StringView host);
    Optional<String> get_public_suffix(StringView string);

};

}

