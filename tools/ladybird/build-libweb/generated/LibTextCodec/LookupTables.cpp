
#include <LibTextCodec/LookupTables.h>

namespace TextCodec {

Optional<u32> index_gb18030_code_point(u32 pointer)
{
    if (pointer >= s_gb18030_index.size())
        return {};
    auto value = s_gb18030_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_gb18030_index(u32 code_point)
{
    for (u32 i = 0; i < s_gb18030_index.size(); ++i) {
        if (s_gb18030_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_big5_code_point(u32 pointer)
{
    if (pointer < s_big5_index_first_pointer || pointer - s_big5_index_first_pointer >= s_big5_index.size())
        return {};
    auto value = s_big5_index[pointer - s_big5_index_first_pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_big5_index(u32 code_point)
{
    for (u32 i = 0; i < s_big5_index.size(); ++i) {
        if (s_big5_index[i] == code_point) {
            return s_big5_index_first_pointer + i;
        }
    }
    return {};
}

Optional<u32> index_jis0208_code_point(u32 pointer)
{
    if (pointer >= s_jis0208_index.size())
        return {};
    auto value = s_jis0208_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_jis0208_index(u32 code_point)
{
    for (u32 i = 0; i < s_jis0208_index.size(); ++i) {
        if (s_jis0208_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_jis0212_code_point(u32 pointer)
{
    if (pointer < s_jis0212_index_first_pointer || pointer - s_jis0212_index_first_pointer >= s_jis0212_index.size())
        return {};
    auto value = s_jis0212_index[pointer - s_jis0212_index_first_pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_jis0212_index(u32 code_point)
{
    for (u32 i = 0; i < s_jis0212_index.size(); ++i) {
        if (s_jis0212_index[i] == code_point) {
            return s_jis0212_index_first_pointer + i;
        }
    }
    return {};
}

Optional<u32> index_euc_kr_code_point(u32 pointer)
{
    if (pointer >= s_euc_kr_index.size())
        return {};
    auto value = s_euc_kr_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_euc_kr_index(u32 code_point)
{
    for (u32 i = 0; i < s_euc_kr_index.size(); ++i) {
        if (s_euc_kr_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_ibm866_code_point(u32 pointer)
{
    if (pointer >= s_ibm866_index.size())
        return {};
    auto value = s_ibm866_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_ibm866_index(u32 code_point)
{
    for (u32 i = 0; i < s_ibm866_index.size(); ++i) {
        if (s_ibm866_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_2022_jp_katakana_code_point(u32 pointer)
{
    if (pointer >= s_iso_2022_jp_katakana_index.size())
        return {};
    auto value = s_iso_2022_jp_katakana_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_2022_jp_katakana_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_2022_jp_katakana_index.size(); ++i) {
        if (s_iso_2022_jp_katakana_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_2_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_2_index.size())
        return {};
    auto value = s_iso_8859_2_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_2_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_2_index.size(); ++i) {
        if (s_iso_8859_2_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_3_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_3_index.size())
        return {};
    auto value = s_iso_8859_3_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_3_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_3_index.size(); ++i) {
        if (s_iso_8859_3_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_4_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_4_index.size())
        return {};
    auto value = s_iso_8859_4_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_4_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_4_index.size(); ++i) {
        if (s_iso_8859_4_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_5_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_5_index.size())
        return {};
    auto value = s_iso_8859_5_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_5_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_5_index.size(); ++i) {
        if (s_iso_8859_5_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_6_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_6_index.size())
        return {};
    auto value = s_iso_8859_6_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_6_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_6_index.size(); ++i) {
        if (s_iso_8859_6_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_7_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_7_index.size())
        return {};
    auto value = s_iso_8859_7_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_7_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_7_index.size(); ++i) {
        if (s_iso_8859_7_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_8_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_8_index.size())
        return {};
    auto value = s_iso_8859_8_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_8_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_8_index.size(); ++i) {
        if (s_iso_8859_8_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_10_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_10_index.size())
        return {};
    auto value = s_iso_8859_10_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_10_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_10_index.size(); ++i) {
        if (s_iso_8859_10_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_13_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_13_index.size())
        return {};
    auto value = s_iso_8859_13_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_13_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_13_index.size(); ++i) {
        if (s_iso_8859_13_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_14_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_14_index.size())
        return {};
    auto value = s_iso_8859_14_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_14_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_14_index.size(); ++i) {
        if (s_iso_8859_14_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_15_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_15_index.size())
        return {};
    auto value = s_iso_8859_15_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_15_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_15_index.size(); ++i) {
        if (s_iso_8859_15_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_iso_8859_16_code_point(u32 pointer)
{
    if (pointer >= s_iso_8859_16_index.size())
        return {};
    auto value = s_iso_8859_16_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_iso_8859_16_index(u32 code_point)
{
    for (u32 i = 0; i < s_iso_8859_16_index.size(); ++i) {
        if (s_iso_8859_16_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_koi8_r_code_point(u32 pointer)
{
    if (pointer >= s_koi8_r_index.size())
        return {};
    auto value = s_koi8_r_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_koi8_r_index(u32 code_point)
{
    for (u32 i = 0; i < s_koi8_r_index.size(); ++i) {
        if (s_koi8_r_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_koi8_u_code_point(u32 pointer)
{
    if (pointer >= s_koi8_u_index.size())
        return {};
    auto value = s_koi8_u_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_koi8_u_index(u32 code_point)
{
    for (u32 i = 0; i < s_koi8_u_index.size(); ++i) {
        if (s_koi8_u_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_macintosh_code_point(u32 pointer)
{
    if (pointer >= s_macintosh_index.size())
        return {};
    auto value = s_macintosh_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_macintosh_index(u32 code_point)
{
    for (u32 i = 0; i < s_macintosh_index.size(); ++i) {
        if (s_macintosh_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_874_code_point(u32 pointer)
{
    if (pointer >= s_windows_874_index.size())
        return {};
    auto value = s_windows_874_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_874_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_874_index.size(); ++i) {
        if (s_windows_874_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1250_code_point(u32 pointer)
{
    if (pointer >= s_windows_1250_index.size())
        return {};
    auto value = s_windows_1250_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1250_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1250_index.size(); ++i) {
        if (s_windows_1250_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1251_code_point(u32 pointer)
{
    if (pointer >= s_windows_1251_index.size())
        return {};
    auto value = s_windows_1251_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1251_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1251_index.size(); ++i) {
        if (s_windows_1251_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1252_code_point(u32 pointer)
{
    if (pointer >= s_windows_1252_index.size())
        return {};
    auto value = s_windows_1252_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1252_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1252_index.size(); ++i) {
        if (s_windows_1252_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1253_code_point(u32 pointer)
{
    if (pointer >= s_windows_1253_index.size())
        return {};
    auto value = s_windows_1253_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1253_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1253_index.size(); ++i) {
        if (s_windows_1253_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1254_code_point(u32 pointer)
{
    if (pointer >= s_windows_1254_index.size())
        return {};
    auto value = s_windows_1254_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1254_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1254_index.size(); ++i) {
        if (s_windows_1254_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1255_code_point(u32 pointer)
{
    if (pointer >= s_windows_1255_index.size())
        return {};
    auto value = s_windows_1255_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1255_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1255_index.size(); ++i) {
        if (s_windows_1255_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1256_code_point(u32 pointer)
{
    if (pointer >= s_windows_1256_index.size())
        return {};
    auto value = s_windows_1256_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1256_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1256_index.size(); ++i) {
        if (s_windows_1256_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1257_code_point(u32 pointer)
{
    if (pointer >= s_windows_1257_index.size())
        return {};
    auto value = s_windows_1257_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1257_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1257_index.size(); ++i) {
        if (s_windows_1257_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_windows_1258_code_point(u32 pointer)
{
    if (pointer >= s_windows_1258_index.size())
        return {};
    auto value = s_windows_1258_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_windows_1258_index(u32 code_point)
{
    for (u32 i = 0; i < s_windows_1258_index.size(); ++i) {
        if (s_windows_1258_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

Optional<u32> index_x_mac_cyrillic_code_point(u32 pointer)
{
    if (pointer >= s_x_mac_cyrillic_index.size())
        return {};
    auto value = s_x_mac_cyrillic_index[pointer];
    if (value == 0xfffd)
        return {};
    return value;
}

Optional<u32> code_point_x_mac_cyrillic_index(u32 code_point)
{
    for (u32 i = 0; i < s_x_mac_cyrillic_index.size(); ++i) {
        if (s_x_mac_cyrillic_index[i] == code_point) {
            return i;
        }
    }
    return {};
}

}
