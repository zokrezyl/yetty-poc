# yecho - Rich Terminal Output with Glyphs and Cards

`yecho` is a command-line tool that outputs rich text with embedded shader glyphs and GPU-rendered cards. It parses a unified grammar (YAST) and outputs UTF-8 text containing special codepoints and OSC escape sequences that yetty renders as animated glyphs and interactive visualizations.

## Quick Start

```bash
# Simple text with glyphs
yecho "Loading @spinner please wait"

# Styled blocks
yecho "{color=#ff0000: Error:} something failed"

# Plot card (yetty-only)
yecho "{card=plot; w=80; h=20; @view=-3.14..3.14,-1..1: f1=sin(x); @f1.color=#00FF00}"

# List available glyphs
yecho --list
```

## Grammar Overview

yecho uses the YAST (Yetty Abstract Syntax Tree) grammar for parsing. The grammar has three main constructs:

| Construct | Syntax | Description |
|-----------|--------|-------------|
| Glyphs | `@name` | Inline shader-rendered animated glyph |
| Styled Blocks | `{attrs: content}` | ANSI-styled text (works in any terminal) |
| Card Blocks | `{card=TYPE; attrs: content}` | GPU-rendered card (yetty-only) |

## Glyphs: `@name`

Insert a shader glyph by name. Glyphs are animated graphics rendered directly by the GPU, each occupying a single character cell.

```bash
yecho "@spinner"           # Spinning loading indicator
yecho "@heart"             # Pulsing heart
yecho "@fire"              # Animated fire effect
yecho "@matrix-rain"       # Matrix-style rain
```

Glyphs are defined as WGSL shader files in `src/yetty/shaders/glyphs/`. Each file like `0x0000-spinner.wgsl` defines both the glyph's codepoint offset and its rendering logic.

## Styled Blocks: `{attrs: content}`

Apply styling to a block of text. The `:` separates attributes from content.

```bash
yecho "{style=bold: Important!}"
yecho "{color=#ff0000: Error message}"
yecho "{style=bold|italic: Emphasized text}"
yecho "{color=#00ff00 bg=#000: Green on black}"
```

**Attributes:**

| Attribute | Example | Description |
|-----------|---------|-------------|
| `color=#RRGGBB` | `color=#ff0000` | Text color (hex) |
| `color=#RGB` | `color=#f00` | Text color (short hex) |
| `bg=#RRGGBB` | `bg=#333333` | Background color |
| `style=bold` | `style=bold` | Bold text |
| `style=italic` | `style=italic` | Italic text |
| `style=underline` | `style=underline` | Underlined text |
| `style=bold\|italic` | `style=bold\|italic` | Combined styles |

Multiple attributes are separated by whitespace. These use ANSI escape sequences and work in any terminal.

### Nesting

Styled blocks can be nested:

```bash
yecho "{color=#0000ff: Blue {style=bold: and bold} text}"
yecho "{style=bold: Click @heart to {color=#ff0000: like}}"
```

## Card Blocks: `{card=TYPE; attrs: content}` (Planned)

Cards are GPU-rendered widgets that span multiple terminal cells. Unlike glyphs (single cell) or styled blocks (ANSI text), cards require yetty's card rendering system.

### Block Structure

```
{card=TYPE; attr1=val1; attr2=val2; @attr3=val3: content}
 ├─ card=TYPE    First statement, identifies card plugin
 ├─ attrs        Semicolon-separated, card-specific attributes
 ├─ :            Separator between context (left) and content (right)
 └─ content      Card payload (expression, text, data depending on card type)
```

**Key syntax rules:**
- Statements on BOTH sides of `:` are separated by semicolons
- No spaces allowed within attribute values (space = end of value)
- `@` prefix for viewport/reference attributes
- First statement MUST be `card=TYPE`

### Plot Card

```bash
# Single function plot
yecho "{card=plot; w=80; h=20; @view=-3.14..3.14,-1..1: f1=sin(x)}"

# Multi-function plot with colors
yecho "{card=plot; w=80; h=20; @view=-3.14..3.14,-1..1: f1=sin(x); @f1.color=#00FF00; f2=cos(x); @f2.color=#FF0000}"

# Animated plot with time
yecho "{card=plot; w=60; h=15; @view=0..10,-1..1: wave=sin(x-@time)}"
```

**Plot attributes:**

| Attribute | Example | Description |
|-----------|---------|-------------|
| `w=N` | `w=80` | Width in cells |
| `h=N` | `h=20` | Height in cells |
| `@view=X1..X2,Y1..Y2` | `@view=-3.14..3.14,-1..1` | Viewport range |

**Plot content (expression grammar):**

| Syntax | Example | Description |
|--------|---------|-------------|
| `name=expr` | `f1=sin(x)` | Define a function |
| `@name.color=#HEX` | `@f1.color=#FF0000` | Set function color |
| `@name.text='label'` | `@f1.text='Sine'` | Set function label |
| `@bufferN` | `@buffer1` | Reference data buffer |
| `@time` | `sin(x-@time)` | Time uniform for animation |

### YText Card

```bash
# Large styled text
yecho "{card=ytext; w=40; h=10; @font-size=48; @fg-color=#FFFFFF; @bg-color=#000000: Hello World}"
```

**YText attributes:**

| Attribute | Example | Description |
|-----------|---------|-------------|
| `w=N`, `h=N` | `w=40; h=10` | Dimensions in cells |
| `@font-size=N` | `@font-size=48` | Font size in pixels |
| `@fg-color=#HEX` | `@fg-color=#FFFFFF` | Foreground color |
| `@bg-color=#HEX` | `@bg-color=#000000` | Background color |

### QR Card

```bash
yecho "{card=qr; w=20; h=20: https://github.com/zokrezyl/yetty}"
```

## Escaping: `\@`, `\{`, `\}`, `\\`

Use backslash to include literal special characters:

```bash
yecho "Email: user\@example.com"      # Literal @
yecho "JSON: \{\"key\": 1\}"          # Literal braces
yecho "Path: C:\\Users\\name"         # Literal backslash
```

## Available Glyphs

Run `yecho --list` to see all available glyphs:

```
@spinner        Loading spinner
@pulse          Pulsing dot
@wave           Wave animation
@fire           Fire effect
@gem            Sparkling gem
@heart          Pulsing heart
@plasma         Plasma effect
@ripple         Water ripple
@rocket         Flying rocket
@star           Twinkling star
@spiral         Spiral animation
@bouncing-ball  Bouncing ball
@radar-sweep    Radar sweep
@hourglass      Hourglass timer
@dna-helix      DNA double helix
@equalizer      Audio equalizer
@orbit          Orbiting dots
@matrix-rain    Matrix code rain
@metaballs      Metaball blobs
@heartbeat      ECG heartbeat
@typing-dots    Typing indicator
@vinyl          Spinning vinyl
...and more
```

## Options

```
yecho [OPTIONS] [text...]

Options:
  -h, --help        Show help message
  -l, --list        List available glyphs
  -n                Don't print trailing newline
  --shader-dir=PATH Load additional glyphs from directory

Arguments:
  text...           Text to echo (multiple args joined with spaces)
```

## Examples

### Status Messages

```bash
yecho "@spinner Building project..."
yecho "@heart {color=#00ff00: Success!}"
yecho "{color=#ff0000: @fire Error:} Build failed"
```

### Progress Indicators

```bash
yecho "@hourglass Processing... @typing-dots"
yecho "@radar-sweep Scanning @signal-bars"
```

### Decorative Headers

```bash
yecho "@star @star @star {style=bold: Welcome} @star @star @star"
```

### Reading from stdin

```bash
echo "Status: @spinner loading" | yecho
cat message.txt | yecho
```

## Implementation Details

### How Glyphs Work

1. `yecho` parses the input text, recognizing `@glyph` patterns
2. Glyph names are mapped to Unicode codepoints in the Private Use Area (U+101000+)
3. Output is UTF-8 text with embedded glyph codepoints
4. When displayed in yetty, the terminal's shader font renders these as animated glyphs

### How Cards Work (Planned)

Cards require generating OSC (Operating System Command) escape sequences:

```
ESC ] 666666 ; generic-args ; plugin-args ; payload ESC \
```

When yecho encounters a card block like:
```
{card=plot; w=80; h=20; @view=-3.14..3.14,-1..1: f1=sin(x); @f1.color=#00FF00}
```

It generates:
```
\033]666666;create -p plot -w 80 -h 20 -r;--expr "f1=sin(x);@f1.color=#00FF00" --view -3.14,3.14,-1,1;\033\\
```

### OSC Parsing Challenge

The OSC format uses semicolons as field separators:
```
666666;generic-args;plugin-args;payload
```

This conflicts with semicolons in the plugin-args (e.g., expressions like `f1=sin(x);@f1.color=#FF0000`).

**Solution:** The OSC parser (`osc-command.cpp`) respects quoted strings when splitting fields. Semicolons inside double or single quotes are NOT treated as field separators:

```cpp
// splitFields() respects quotes
if (c == '"' || c == '\'') {
    inQuotes = !inQuotes;
} else if (c == ';' && !inQuotes) {
    // Only split here
}
```

Therefore, yecho must quote the expression when generating OSC:
```
--expr "f1=sin(x);@f1.color=#00FF00"
```

### Two Syntaxes: CLI vs YAST

yetty supports two ways to create cards:

1. **CLI syntax** (existing, for scripts/programs):
   ```bash
   printf '\033]666666;create -p plot -w 80 -h 20 -r;--expr "f1=sin(x)" --grid --axes;\033\\'
   ```

2. **YAST syntax** (planned, for yecho):
   ```bash
   yecho "{card=plot; w=80; h=20: f1=sin(x)}"
   ```

yecho translates YAST card blocks into CLI-style OSC sequences internally. The card plugins themselves don't need to change - they receive the same CLI args via OSC.

## Shell Completion

Enable tab completion for glyph names and options:

### Bash

```bash
# Add to ~/.bashrc
eval "$(yecho --bash-completion)"
```

### Zsh

```bash
# Add to ~/.zshrc
eval "$(yecho --zsh-completion)"
```

After sourcing your shell config, you can use:
- `yecho @sp<TAB>` → completes to `@spinner`, `@spiral`, etc.
- `yecho --<TAB>` → shows `--help`, `--list`, `--shader-dir=`

## See Also

- [cards.md](cards.md) - Card system architecture and OSC protocol
- [plot-enhanced.md](plot-enhanced.md) - Expression-based plotting
- [Shader Glyphs](shader-glyphs.md) - Creating custom animated glyphs
