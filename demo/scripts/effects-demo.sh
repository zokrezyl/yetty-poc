#!/bin/bash
# Demo: all pre/post-processing effects
# Usage: source this script from within yetty

echo "=== Post-Processing Effects Demo ==="
echo ""
echo "The quick brown fox jumps over the lazy dog."
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789"
echo "Hello, World! This is a test of post-processing effects."
echo ""

[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 1 || true

echo "--- [post 1] Scanlines ---"
printf '\e]666668;1:0.3:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 2] CRT monitor ---"
printf '\e]666668;2:0.4:0.15:0.3\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 3] Chromatic aberration ---"
printf '\e]666668;3:0.5\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 4] Broken TV ---"
printf '\e]666668;4:0.5:0.3:1.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 5] Matrix ---"
printf '\e]666668;5:0.8:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 6] Sepia ---"
printf '\e]666668;6:0.8:0.1\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 7] Pixelate ---"
printf '\e]666668;7:4.0:8.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 8] Wave ---"
printf '\e]666668;8:0.3:0.05:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 9] Invert ---"
printf '\e]666668;9:1.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 10] Night vision ---"
printf '\e]666668;10:0.9:0.15:0.6\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 11] Vaporwave ---"
printf '\e]666668;11:0.4:0.5\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 12] Thermal camera ---"
printf '\e]666668;12:1.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 13] Glitch ---"
printf '\e]666668;13:0.5:20.0:5.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 14] Emboss ---"
printf '\e]666668;14:1.0:0.785\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 15] Rain overlay ---"
printf '\e]666668;15:0.3:3.0:0.15\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 16] Matrix rain (MSDF rendered) ---"
printf '\e]666668;16:0.5:1.5:15.0:0.9:0.5\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 5 || true

echo "--- [post 17] Drip (color streaks) ---"
printf '\e]666668;17:0.8:10.0:0.8:0.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [post 17] Drip (glyph mode) ---"
printf '\e]666668;17:0.8:10.0:0.8:1.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

printf '\e]666668;0\a'
echo ""
echo "--- Pre-effects ---"
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 1 || true

echo "--- [pre 1] Scramble ---"
printf '\e]666667;1:0.3:1.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [pre 2] Rain characters ---"
printf '\e]666667;2:0.15:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [pre 3] Wave text ---"
printf '\e]666667;3:0.3:0.3:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [pre 4] Matrix falling characters ---"
printf '\e]666667;4:0.4:1.5:12.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 3 || true

echo "--- [combo] Matrix pre + Matrix post ---"
printf '\e]666667;4:0.4:1.5:12.0\a'
printf '\e]666668;5:0.8:2.0\a'
[[ -z "$YETTY_DEMO_NO_SLEEP" ]] && sleep 5 || true

echo "--- Disabling all effects ---"
printf '\e]666667;0\a'
printf '\e]666668;0\a'
echo ""
echo "Done! 15 post-effects + 4 pre-effects."
