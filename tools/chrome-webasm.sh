!/usr/bin/env bash


google-chrome \
  --kiosk \
  --no-first-run \
  --user-data-dir=/tmp/chrome-kiosk \
  --enable-gpu \
  --ignore-gpu-blocklist \
  --enable-unsafe-webgpu \
  --enable-features=Vulkan,UseSkiaRenderer \
  --use-vulkan \
  --ozone-platform=wayland \
  https://zokrezyl.github.io/yetty
