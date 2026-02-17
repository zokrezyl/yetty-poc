#include <yetty/font/freetype.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace yetty::font {

FT_Library ftLibrary() {
    thread_local struct FTLib {
        FT_Library lib = nullptr;
        FTLib()  { FT_Init_FreeType(&lib); }
        ~FTLib() { if (lib) FT_Done_FreeType(lib); }
    } instance;
    return instance.lib;
}

} // namespace yetty::font
