//=========================================================================
// Text selection
//=========================================================================

void buildGlyphSortedOrder() override {
  _glyphSortedOrder.resize(_glyphs.size());
  std::iota(_glyphSortedOrder.begin(), _glyphSortedOrder.end(), 0u);
  std::sort(_glyphSortedOrder.begin(), _glyphSortedOrder.end(),
            [this](uint32_t a, uint32_t b) {
              const auto &ga = _glyphs[a];
              const auto &gb = _glyphs[b];
              if (ga.y != gb.y)
                return ga.y < gb.y;
              return ga.x < gb.x;
            });
}

int32_t findNearestGlyphSorted(float sceneX, float sceneY) override {
  if (_glyphSortedOrder.empty())
    return -1;

  float bestDist = std::numeric_limits<float>::max();
  int32_t bestSorted = -1;

  for (size_t si = 0; si < _glyphSortedOrder.size(); si++) {
    const auto &g = _glyphs[_glyphSortedOrder[si]];
    float gw = g.width();
    float gh = g.height();

    if (sceneX >= g.x && sceneX < g.x + gw && sceneY >= g.y &&
        sceneY < g.y + gh) {
      return static_cast<int32_t>(si);
    }

    float cx = g.x + gw * 0.5f;
    float cy = g.y + gh * 0.5f;
    float dist =
        (cx - sceneX) * (cx - sceneX) + (cy - sceneY) * (cy - sceneY);
    if (dist < bestDist) {
      bestDist = dist;
      bestSorted = static_cast<int32_t>(si);
    }
  }
  return bestSorted;
}

void setSelectionRange(int32_t startSorted, int32_t endSorted) override {
  for (auto &g : _glyphs) {
    g.glyphLayerFlags &= ~(static_cast<uint32_t>(GLYPH_FLAG_SELECTED) << 24);
  }

  if (startSorted < 0 || endSorted < 0 || _glyphSortedOrder.empty()) {
    _bufferDirty = true;
    return;
  }

  int32_t lo = std::min(startSorted, endSorted);
  int32_t hi = std::min(std::max(startSorted, endSorted),
                        static_cast<int32_t>(_glyphSortedOrder.size()) - 1);

  for (int32_t i = lo; i <= hi; i++) {
    _glyphs[_glyphSortedOrder[i]].glyphLayerFlags |=
        (static_cast<uint32_t>(GLYPH_FLAG_SELECTED) << 24);
  }
  _bufferDirty = true;
}

std::string getSelectedText() override {
  if (_glyphSortedOrder.empty())
    return {};

  std::string result;
  float prevY = -1e30f;
  float prevEndX = -1e30f;
  bool first = true;

  for (size_t i = 0; i < _glyphSortedOrder.size(); i++) {
    const auto &g = _glyphs[_glyphSortedOrder[i]];
    if (((g.glyphLayerFlags >> 24) & GLYPH_FLAG_SELECTED) == 0)
      continue;

    float gh = g.height();
    float gw = g.width();

    if (!first) {
      if ((g.y - prevY) > gh * 0.5f) {
        result += '\n';
        prevEndX = -1e30f;
      } else if (prevEndX > -1e29f && (g.x - prevEndX) > gw * 0.3f) {
        result += ' ';
      }
    }
    first = false;
    prevY = g.y;
    prevEndX = g.x + gw;

    uint16_t glyphIdx = static_cast<uint16_t>(g.glyphLayerFlags & 0xFFFF);
    bool isCustom =
        ((g.glyphLayerFlags >> 24) & GLYPH_FLAG_CUSTOM_ATLAS) != 0;

    uint32_t cp = 0;
    if (isCustom && _customAtlas) {
      cp = _customAtlas->getCodepoint(glyphIdx);
    } else if (_atlas) {
      cp = _atlas->getCodepoint(glyphIdx);
    }

    if (cp > 0) {
      if (cp < 0x80) {
        result += static_cast<char>(cp);
      } else if (cp < 0x800) {
        result += static_cast<char>(0xC0 | (cp >> 6));
        result += static_cast<char>(0x80 | (cp & 0x3F));
      } else if (cp < 0x10000) {
        result += static_cast<char>(0xE0 | (cp >> 12));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
      } else {
        result += static_cast<char>(0xF0 | (cp >> 18));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
      }
    }
  }
  return result;
}
