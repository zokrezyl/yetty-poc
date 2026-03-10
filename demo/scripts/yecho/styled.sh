#!/bin/bash
# YEcho Styled: ANSI colored and styled text blocks
DIR="$(cd "$(dirname "$0")" && pwd)"
YECHO="$DIR/../../../build-desktop-dawn-release/src/yetty/yecho/yecho"

echo "=== YEcho Styled Text Demo ==="
echo

# Colors
$YECHO "{color=#ff0000: Red text}"
echo
$YECHO "{color=#00ff00: Green text}"
echo
$YECHO "{color=#0000ff: Blue text}"
echo
$YECHO "{color=#ff00ff: Magenta text}"
echo

# Background colors
$YECHO "{bg=#ffff00: Yellow background}"
echo
$YECHO "{bg=#00ffff: Cyan background}"
echo

# Styles
$YECHO "{style=bold: Bold text}"
echo
$YECHO "{style=italic: Italic text}"
echo
$YECHO "{style=underline: Underlined text}"
echo

# Combined
$YECHO "{color=#ff0000; style=bold: Bold red text}"
echo
$YECHO "{color=#00ff00; bg=#000000; style=italic: Green italic on black}"
echo
$YECHO "{style=bold|underline: Bold and underlined}"
echo

# Mixed with regular text
$YECHO "Status: {color=#00ff00: PASSED} - All tests completed"
echo
$YECHO "{color=#ff0000: ERROR}: Something went wrong at {style=bold: line 42}"
echo
