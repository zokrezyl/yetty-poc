#!/bin/bash
# Query terminal cell size using ANSI escape sequences
# CSI 14 t - window size in pixels
# CSI 18 t - text area in characters

# Save terminal settings and set raw mode
old_settings=$(stty -g)
stty raw -echo min 0 time 1

# Query window size in pixels (CSI 14 t)
printf '\033[14t'
read -r -d 't' pixel_response
pixel_response="${pixel_response#*[}"  # Remove ESC[

# Query text area in characters (CSI 18 t)
printf '\033[18t'
read -r -d 't' char_response
char_response="${char_response#*[}"  # Remove ESC[

# Restore terminal settings
stty "$old_settings"

# Parse responses: format is "4;height;width" for pixels, "8;rows;cols" for chars
IFS=';' read -r _ pixel_height pixel_width <<< "$pixel_response"
IFS=';' read -r _ char_rows char_cols <<< "$char_response"

# Calculate cell size
if [[ -n "$pixel_width" && -n "$char_cols" && "$char_cols" -gt 0 ]]; then
    cell_width=$((pixel_width / char_cols))
else
    cell_width="?"
fi

if [[ -n "$pixel_height" && -n "$char_rows" && "$char_rows" -gt 0 ]]; then
    cell_height=$((pixel_height / char_rows))
else
    cell_height="?"
fi

echo "Window pixels: ${pixel_width}x${pixel_height}"
echo "Text area:     ${char_cols}x${char_rows} cells"
echo "Cell size:     ${cell_width}x${cell_height} pixels"
