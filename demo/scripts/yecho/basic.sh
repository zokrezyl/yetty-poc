#!/bin/bash
# YEcho Basic: Simple text output with glyphs
YECHO="${YECHO:-yecho}"

echo "=== YEcho Basic Demo ==="
echo

# Plain text
$YECHO "Hello, World!"
echo

# With glyphs
$YECHO "Loading @spinner please wait..."
echo

$YECHO "Status: @check Done!"
echo

$YECHO "@warning Warning: Disk space low"
echo

$YECHO "@error Error occurred @arrow-right check logs"
echo
