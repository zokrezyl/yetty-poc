//=============================================================================
// Read font ascender/descender from TTF hhea + head tables (no FreeType)
//=============================================================================
struct TtfMetrics {
  float ascender;
  float descender;
};

static TtfMetrics readTtfMetrics(const std::string &path, float fontSize) {
  TtfMetrics fallback{fontSize * 0.8f, fontSize * 0.2f};
  std::ifstream f(path, std::ios::binary);
  if (!f)
    return fallback;

  auto r16 = [&](uint32_t off) -> uint16_t {
    char b[2];
    f.seekg(off);
    f.read(b, 2);
    return (uint8_t(b[0]) << 8) | uint8_t(b[1]);
  };
  auto ri16 = [&](uint32_t off) -> int16_t {
    return static_cast<int16_t>(r16(off));
  };

  uint16_t numTables = r16(4);
  uint32_t hheaOff = 0, headOff = 0;
  for (uint16_t i = 0; i < numTables; i++) {
    uint32_t entry = 12 + i * 16;
    char tag[4];
    f.seekg(entry);
    f.read(tag, 4);
    uint32_t off =
        (static_cast<uint32_t>(r16(entry + 8)) << 16) | r16(entry + 10);
    if (std::memcmp(tag, "hhea", 4) == 0)
      hheaOff = off;
    if (std::memcmp(tag, "head", 4) == 0)
      headOff = off;
  }
  if (!hheaOff || !headOff)
    return fallback;

  uint16_t upm = r16(headOff + 18);
  int16_t asc = ri16(hheaOff + 4);
  int16_t desc = ri16(hheaOff + 6);
  if (upm == 0)
    return fallback;

  float scale = fontSize / static_cast<float>(upm);
  return {asc * scale, std::abs(desc) * scale};
}
