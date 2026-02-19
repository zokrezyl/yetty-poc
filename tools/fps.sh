#!/bin/bash
# Set yetty rendering frame rate via OSC 666671
printf '\033]666671;%s\033\\' "${1:-60}"
