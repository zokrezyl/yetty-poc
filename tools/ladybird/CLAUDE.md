# LibWeb-to-YDraw Build Instructions

## Building

**For development (with ccache - USE THIS):**
```bash
./build-dev.sh
```
Uses ccache for fast incremental builds. Build artifacts persist in `build-dev/`.

**For clean/release build (slow - remote nix):**
```bash
./build.sh
```
Full nix rebuild on remote builder. Only use for final testing.

## NEVER DO

- **NEVER** run ad-hoc SSH commands to the remote builder (192.168.1.10)
- **NEVER** use `--builders` flag manually
- **NEVER** ping or test SSH connectivity directly
- **NEVER** use `nix build` repeatedly for small changes - use `build-dev.sh` instead

## Scripts

| Script | Purpose |
|--------|---------|
| `build-dev.sh` | Incremental build with ccache (FAST) |
| `build.sh` | Full nix remote build (SLOW) |

## Troubleshooting

If remote builder fails, check:
1. `/var/log/nix/daemon.log` on local machine
2. Wait if blocked by fail2ban (usually 10 minutes)

See `docs/nix-remote-build.md` for full configuration details.
