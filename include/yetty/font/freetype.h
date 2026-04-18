#pragma once

typedef struct FT_LibraryRec_* FT_Library;

namespace yetty::font {

/// Thread-local FreeType library singleton.
/// FT_Library is not thread-safe, so one instance per thread.
FT_Library ftLibrary();

} // namespace yetty::font
