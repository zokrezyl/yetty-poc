# Yetty Nix Build System

Reproducible builds via Nix using Clang. Compatible with remote Nix builders.

## Structure

```
build-tools/nix/
├── default.nix       # Package derivation (uses clangStdenv)
├── cpm-sources.nix   # Pre-fetched CPM dependencies (auto-generated)
├── versions.txt      # Dependency versions (single source of truth)
├── update-deps.sh    # Regenerates cpm-sources.nix from versions.txt
└── README.md
```

## Usage

```bash
# Build
nix build .#yetty

# Run
nix run .#yetty

# Development shell
nix develop

# Build with full logging
nix build .#yetty-debug
```

## Updating Dependencies

1. Edit `versions.txt` with new versions
2. Run `./update-deps.sh`
3. Test: `nix build .#yetty`

### versions.txt Format

```
name|type|source|version
```

Types:
- `github` - GitHub repo: `owner/repo|tag_or_commit`
- `url` - Direct URL: `https://...tar.gz|version_label`

## Remote Builds

Works with any Nix remote builder:

```bash
nix build .#yetty --builders 'ssh://builder x86_64-linux'
```

## Compiler

Uses Clang via `clangStdenv` (LLVM 18). Windows uses MSVC.
