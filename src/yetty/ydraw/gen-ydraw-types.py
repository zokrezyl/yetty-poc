#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.10"
# dependencies = ["pyyaml"]
# ///
"""Generate C++ and WGSL code from ydraw-primitives.yaml.

Run:  uv run src/yetty/ydraw/gen-ydraw-types.py

Reads:    src/yetty/ydraw/ydraw-primitives.yaml
Generates:
  src/yetty/cards/hdraw/ydraw-types.gen.h   — C++ enum + field offsets
  src/yetty/shaders/lib/sdf-types.gen.wgsl  — WGSL constants + compact dispatch
  src/yetty/ydraw/ydraw-prim-writer.gen.h   — C++ per-type writer functions
"""

import re
import sys
from pathlib import Path

import yaml

SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parent.parent.parent  # src/yetty/ydraw -> project root

YAML_PATH = SCRIPT_DIR / "ydraw-primitives.yaml"
CPP_OUT = SCRIPT_DIR / "ydraw-types.gen.h"
WGSL_OUT = PROJECT_ROOT / "src" / "yetty" / "shaders" / "lib" / "sdf-types.gen.wgsl"
WRITER_OUT = SCRIPT_DIR / "ydraw-prim-writer.gen.h"
BUFFER_OUT = SCRIPT_DIR / "ydraw-buffer.gen.inc"
CAPI_H_OUT = SCRIPT_DIR / "ydraw-capi.gen.h"
CAPI_C_OUT = SCRIPT_DIR / "ydraw-capi.gen.c"
CAPI_WRAPPER_OUT = SCRIPT_DIR / "ydraw-capi-wrapper.gen.cpp"

HEADER = "// Auto-generated from ydraw-primitives.yaml — DO NOT EDIT\n"

# Fields in SDFPrimitive that have dedicated struct members (not in params[])
SPECIAL_FIELDS = {"type", "layer", "fillColor", "strokeColor", "strokeWidth", "round"}


# =============================================================================
# Name conversion
# =============================================================================

def camel_to_screaming_snake(name: str) -> str:
    """CamelCase → SCREAMING_SNAKE_CASE."""
    s = re.sub(r"([a-z])([A-Z])", r"\1_\2", name)
    s = re.sub(r"([a-zA-Z])(\d+[A-Z])", r"\1_\2", s)
    return s.upper()


def camel_to_snake(name: str) -> str:
    """CamelCase → snake_case."""
    s = re.sub(r"([a-z])([A-Z])", r"\1_\2", name)
    s = re.sub(r"([a-zA-Z])(\d+[A-Z])", r"\1_\2", s)
    return s.lower()


# =============================================================================
# YAML loading + enrichment
# =============================================================================

def load_primitives(path: Path) -> list[dict]:
    with open(path) as f:
        data = yaml.safe_load(f)

    primitives = data["primitives"]

    for prim in primitives:
        fields = prim.get("fields", [])
        offset_map: dict[str, tuple[int, str]] = {}
        for i, field in enumerate(fields):
            offset_map[field["name"]] = (i, field["type"])
        prim["_offset_map"] = offset_map
        prim["_word_count"] = len(fields)
        prim["_const_name"] = "SDF_" + camel_to_screaming_snake(prim["name"])

        # Compute params[] index for each geometry field (non-special)
        param_idx = 0
        prim["_param_map"] = {}  # field_name -> params index
        for field in fields:
            if field["name"] not in SPECIAL_FIELDS:
                prim["_param_map"][field["name"]] = param_idx
                param_idx += 1

    return primitives


# =============================================================================
# Field substitution for WGSL eval blocks
# =============================================================================

def substitute_fields(eval_code: str, offset_map: dict) -> str:
    """{fieldName} → cardStorage[primOffset + Nu] (with bitcast for u32)."""

    def _repl(m):
        name = m.group(1)
        if name not in offset_map:
            return m.group(0)
        offset, ftype = offset_map[name]
        if ftype == "u32":
            return f"bitcast<u32>(cardStorage[primOffset + {offset}u])"
        return f"cardStorage[primOffset + {offset}u]"

    return re.sub(r"\{(\w+)\}", _repl, eval_code)


# =============================================================================
# C++ generation — enum + field offsets
# =============================================================================

def generate_cpp(primitives: list[dict], out: Path) -> None:
    L = []
    L.append(HEADER)
    L.append("#pragma once\n")
    L.append("#include <cstdint>\n")
    L.append("namespace yetty::card {\n")

    # --- enum ---
    L.append("enum class SDFType : uint32_t {")
    for prim in primitives:
        L.append(f"    {prim['name']} = {prim['id']},")
    L.append("};\n")

    # --- per-type field offsets ---
    L.append("namespace sdf_field {\n")
    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue
        L.append(f"namespace {prim['name']} {{")
        for field in fields:
            off = prim["_offset_map"][field["name"]][0]
            cname = "k" + field["name"][0].upper() + field["name"][1:]
            L.append(f"    constexpr uint32_t {cname} = {off};")
        L.append(f"    constexpr uint32_t kWordCount = {prim['_word_count']};")
        L.append("}\n")
    L.append("} // namespace sdf_field\n")

    L.append("} // namespace yetty::card")
    L.append("")

    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(L))


# =============================================================================
# WGSL generation — constants + compact dispatch (renamed functions)
# =============================================================================

def generate_wgsl(primitives: list[dict], out: Path) -> None:
    L = []
    L.append(HEADER)

    # --- constants ---
    for prim in primitives:
        L.append(f"const {prim['_const_name']}: u32 = {prim['id']}u;")
    L.append("")

    # --- evaluateYdrawSDF (2D) — compact layout dispatch ---
    sdf2d = [p for p in primitives if p["category"] == "sdf2d"]
    if sdf2d:
        L.append("fn evaluateYdrawSDF(primOffset: u32, p: vec2<f32>) -> f32 {")
        L.append("    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);")
        L.append("")
        L.append("    switch (primType) {")
        for prim in sdf2d:
            eval_code = prim.get("eval", "").strip()
            if not eval_code:
                continue
            substituted = substitute_fields(eval_code, prim["_offset_map"])
            L.append(f"        case {prim['_const_name']}: {{")
            for line in substituted.split("\n"):
                L.append(f"            {line}")
            L.append("        }")
        L.append("        default: {")
        L.append("            return 1e10;")
        L.append("        }")
        L.append("    }")
        L.append("}\n")

    # --- evaluateYdrawSDF3D ---
    sdf3d = [p for p in primitives if p["category"] == "sdf3d"]
    if sdf3d:
        L.append("fn evaluateYdrawSDF3D(primOffset: u32, p: vec3<f32>) -> f32 {")
        L.append("    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);")
        L.append("")
        L.append("    switch (primType) {")
        for prim in sdf3d:
            eval_code = prim.get("eval", "").strip()
            if not eval_code:
                continue
            substituted = substitute_fields(eval_code, prim["_offset_map"])
            L.append(f"        case {prim['_const_name']}: {{")
            for line in substituted.split("\n"):
                L.append(f"            {line}")
            L.append("        }")
        L.append("        default: { return 1e10; }")
        L.append("    }")
        L.append("}\n")

    # --- primColors ---
    # Include regular primitives with fillColor
    renderable = [
        p for p in primitives
        if "fillColor" in p["_offset_map"]
        and "strokeColor" in p["_offset_map"]
        and "layer" in p["_offset_map"]
    ]
    # Also include gradient primitives (use color1 as fillColor equivalent)
    gradient_prims = [
        p for p in primitives
        if "color1" in p["_offset_map"]
        and "strokeColor" in p["_offset_map"]
        and "layer" in p["_offset_map"]
    ]
    all_colorable = renderable + gradient_prims
    if all_colorable:
        L.append("fn primColors(primOffset: u32) -> vec4<u32> {")
        L.append("    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);")
        L.append("    switch (primType) {")
        for prim in renderable:
            om = prim["_offset_map"]
            fill_off = om["fillColor"][0]
            stroke_off = om["strokeColor"][0]
            layer_off = om["layer"][0]
            L.append(f"        case {prim['_const_name']}: {{")
            L.append(f"            return vec4<u32>(")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {fill_off}u]),")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {stroke_off}u]),")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {layer_off}u]),")
            L.append(f"                0u);")
            L.append(f"        }}")
        for prim in gradient_prims:
            om = prim["_offset_map"]
            # For gradients, use color1 as fill color (allows basic rendering)
            color1_off = om["color1"][0]
            stroke_off = om["strokeColor"][0]
            layer_off = om["layer"][0]
            L.append(f"        case {prim['_const_name']}: {{")
            L.append(f"            return vec4<u32>(")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {color1_off}u]),")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {stroke_off}u]),")
            L.append(f"                bitcast<u32>(cardStorage[primOffset + {layer_off}u]),")
            L.append(f"                1u);  // flag=1 indicates gradient type")
            L.append(f"        }}")
        L.append("        default: { return vec4<u32>(0u); }")
        L.append("    }")
        L.append("}\n")

    # --- isGradientPrim ---
    if gradient_prims:
        L.append("fn isGradientPrim(primType: u32) -> bool {")
        L.append("    switch (primType) {")
        for prim in gradient_prims:
            L.append(f"        case {prim['_const_name']}: {{ return true; }}")
        L.append("        default: { return false; }")
        L.append("    }")
        L.append("}\n")

    # --- evalGradientFillColor ---
    # Computes gradient color for a given position
    if gradient_prims:
        L.append("fn evalGradientFillColor(primOffset: u32, p: vec2<f32>) -> vec4<f32> {")
        L.append("    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);")
        L.append("    switch (primType) {")
        for prim in gradient_prims:
            om = prim["_offset_map"]
            name = prim["name"]
            if "LinearGradient" in name:
                gx1_off = om["gx1"][0]
                gy1_off = om["gy1"][0]
                gx2_off = om["gx2"][0]
                gy2_off = om["gy2"][0]
                c1_off = om["color1"][0]
                c2_off = om["color2"][0]
                L.append(f"        case {prim['_const_name']}: {{")
                L.append(f"            let gStart = vec2<f32>(cardStorage[primOffset + {gx1_off}u], cardStorage[primOffset + {gy1_off}u]);")
                L.append(f"            let gEnd = vec2<f32>(cardStorage[primOffset + {gx2_off}u], cardStorage[primOffset + {gy2_off}u]);")
                L.append(f"            let gDir = gEnd - gStart;")
                L.append(f"            let gLen = length(gDir);")
                L.append(f"            let t = clamp(dot(p - gStart, gDir) / (gLen * gLen), 0.0, 1.0);")
                L.append(f"            let c1 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + {c1_off}u]));")
                L.append(f"            let c2 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + {c2_off}u]));")
                L.append(f"            return mix(c1, c2, t);")
                L.append(f"        }}")
            elif "RadialGradient" in name:
                gcx_off = om["gcx"][0]
                gcy_off = om["gcy"][0]
                gr_off = om["gr"][0]
                c1_off = om["color1"][0]
                c2_off = om["color2"][0]
                L.append(f"        case {prim['_const_name']}: {{")
                L.append(f"            let gCenter = vec2<f32>(cardStorage[primOffset + {gcx_off}u], cardStorage[primOffset + {gcy_off}u]);")
                L.append(f"            let gRadius = cardStorage[primOffset + {gr_off}u];")
                L.append(f"            let t = clamp(length(p - gCenter) / gRadius, 0.0, 1.0);")
                L.append(f"            let c1 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + {c1_off}u]));")
                L.append(f"            let c2 = unpack4x8unorm(bitcast<u32>(cardStorage[primOffset + {c2_off}u]));")
                L.append(f"            return mix(c1, c2, t);")
                L.append(f"        }}")
        L.append("        default: { return vec4<f32>(0.0); }")
        L.append("    }")
        L.append("}\n")

    # --- primStrokeWidth ---
    has_stroke = [p for p in primitives if "strokeWidth" in p["_offset_map"]]
    if has_stroke:
        L.append("fn primStrokeWidth(primOffset: u32) -> f32 {")
        L.append("    let primType = bitcast<u32>(cardStorage[primOffset + 0u]);")
        L.append("    switch (primType) {")
        for prim in has_stroke:
            sw_off = prim["_offset_map"]["strokeWidth"][0]
            L.append(f"        case {prim['_const_name']}: {{ return cardStorage[primOffset + {sw_off}u]; }}")
        L.append("        default: { return 0.0; }")
        L.append("    }")
        L.append("}")
        L.append("")

    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(L))


# =============================================================================
# C++ writer generation — per-type inline write functions
# =============================================================================

def generate_writer(primitives: list[dict], out: Path) -> None:
    L = []
    L.append(HEADER)
    L.append("#pragma once\n")
    L.append("#include <cstdint>")
    L.append("#include <cstring>")
    L.append("#include <vector>\n")
    L.append("// Forward declaration — include ydraw-types.gen.h for full enum")
    L.append("namespace yetty::card { enum class SDFType : uint32_t; struct SDFPrimitive; }\n")
    L.append("namespace yetty::sdf {\n")
    L.append("namespace detail {")
    L.append("inline void write_u32(float* buf, uint32_t off, uint32_t val) {")
    L.append("    std::memcpy(&buf[off], &val, sizeof(uint32_t));")
    L.append("}")
    L.append("inline uint32_t read_u32(const float* buf, uint32_t off) {")
    L.append("    uint32_t v; std::memcpy(&v, &buf[off], sizeof(uint32_t)); return v;")
    L.append("}")
    L.append("} // namespace detail\n")

    # --- Per-type writer functions ---
    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue
        fname = "write" + prim["name"]

        # Build parameter list (skip 'type' — it's implicit)
        params = []
        for field in fields:
            if field["name"] == "type":
                continue
            # Avoid C++ keyword clash
            pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
            if field["type"] == "u32":
                params.append(f"uint32_t {pname}")
            else:
                params.append(f"float {pname}")

        param_str = ", ".join(params)

        L.append(f"/// Write {prim['name']} ({prim['_word_count']} words). Returns word count.")
        L.append(f"inline uint32_t {fname}(float* buf, {param_str}) {{")

        # Write each field
        for field in fields:
            off = prim["_offset_map"][field["name"]][0]
            if field["name"] == "type":
                L.append(f"    detail::write_u32(buf, {off}, {prim['id']}u);")
            else:
                pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
                if field["type"] == "u32":
                    L.append(f"    detail::write_u32(buf, {off}, {pname});")
                else:
                    L.append(f"    buf[{off}] = {pname};")

        L.append(f"    return {prim['_word_count']};")
        L.append("}\n")

    # --- wordCountForType: type ID → word count ---
    L.append("/// Return word count for a given SDF type ID. 0 = unknown.")
    L.append("inline uint32_t wordCountForType(uint32_t type) {")
    L.append("    switch (type) {")
    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue
        L.append(f"    case {prim['id']}u: return {prim['_word_count']}; // {prim['name']}")
    L.append("    default: return 0;")
    L.append("    }")
    L.append("}\n")

    # --- translateGridEntries: does NOT use SDFPrimitive, always available ---
    L.append("/// Translate grid entries from primitive indices to word offsets.")
    L.append("/// Grid layout: [off0..offN-1][packed_cells...] where cell = [count][e0][e1]...")
    L.append("/// Non-glyph entries (prim indices) are replaced with word offsets.")
    L.append("inline void translateGridEntries(")
    L.append("        uint32_t* grid, uint32_t gridSize,")
    L.append("        uint32_t gridW, uint32_t gridH,")
    L.append("        const std::vector<uint32_t>& wordOffsets) {")
    L.append("    if (wordOffsets.empty() || gridSize == 0) return;")
    L.append("    uint32_t numCells = gridW * gridH;")
    L.append("    if (numCells > gridSize) return;")
    L.append("    for (uint32_t ci = 0; ci < numCells; ci++) {")
    L.append("        uint32_t packedOff = grid[ci];")
    L.append("        if (packedOff >= gridSize) continue;")
    L.append("        uint32_t cnt = grid[packedOff];")
    L.append("        for (uint32_t j = 0; j < cnt; j++) {")
    L.append("            uint32_t idx = packedOff + 1 + j;")
    L.append("            if (idx >= gridSize) break;")
    L.append("            uint32_t rawVal = grid[idx];")
    L.append("            if ((rawVal & 0x80000000u) != 0) continue;")
    L.append("            if (rawVal < static_cast<uint32_t>(wordOffsets.size())) {")
    L.append("                grid[idx] = wordOffsets[rawVal];")
    L.append("            }")
    L.append("        }")
    L.append("    }")
    L.append("}\n")

    # --- SDFPrimitive-dependent functions (need hdraw.h included first) ---
    L.append("#ifdef YETTY_CARD_SDF_PRIMITIVE_DEFINED\n")

    # --- readPrimitive: buffer → SDFPrimitive ---
    L.append("/// Read buffer into SDFPrimitive. Returns words consumed (0 = unknown type).")
    L.append("inline uint32_t readPrimitive(const float* buf, card::SDFPrimitive& prim) {")
    L.append("    std::memset(&prim, 0, sizeof(prim));")
    L.append("    uint32_t primType = detail::read_u32(buf, 0);")
    L.append("    switch (static_cast<card::SDFType>(primType)) {")

    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue
        L.append(f"    case card::SDFType::{prim['name']}: {{")
        for field in fields:
            off = prim["_offset_map"][field["name"]][0]
            fname = field["name"]
            ftype = field["type"]

            if fname == "type":
                L.append(f"        prim.type = detail::read_u32(buf, {off});")
            elif fname == "layer":
                L.append(f"        prim.layer = detail::read_u32(buf, {off});")
            elif fname == "fillColor":
                L.append(f"        prim.fillColor = detail::read_u32(buf, {off});")
            elif fname == "strokeColor":
                L.append(f"        prim.strokeColor = detail::read_u32(buf, {off});")
            elif fname == "strokeWidth":
                L.append(f"        prim.strokeWidth = buf[{off}];")
            elif fname == "round":
                L.append(f"        prim.round = buf[{off}];")
            else:
                # Geometry param
                pidx = prim["_param_map"][fname]
                if ftype == "u32":
                    L.append(f"        std::memcpy(&prim.params[{pidx}], &buf[{off}], sizeof(float));")
                else:
                    L.append(f"        prim.params[{pidx}] = buf[{off}];")

        L.append(f"        return {prim['_word_count']};")
        L.append("    }")

    L.append("    default:")
    L.append("        return 0;")
    L.append("    }")
    L.append("}\n")

    # --- writePrimitive: SDFPrimitive → buffer ---
    L.append("/// Write SDFPrimitive to buffer. Returns word count (0 = unknown type).")
    L.append("inline uint32_t writePrimitive(float* buf, const card::SDFPrimitive& prim) {")
    L.append("    switch (static_cast<card::SDFType>(prim.type)) {")

    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue
        L.append(f"    case card::SDFType::{prim['name']}: {{")
        for field in fields:
            off = prim["_offset_map"][field["name"]][0]
            fname = field["name"]
            ftype = field["type"]

            if fname == "type":
                L.append(f"        detail::write_u32(buf, {off}, prim.type);")
            elif fname == "layer":
                L.append(f"        detail::write_u32(buf, {off}, prim.layer);")
            elif fname == "fillColor":
                L.append(f"        detail::write_u32(buf, {off}, prim.fillColor);")
            elif fname == "strokeColor":
                L.append(f"        detail::write_u32(buf, {off}, prim.strokeColor);")
            elif fname == "strokeWidth":
                L.append(f"        buf[{off}] = prim.strokeWidth;")
            elif fname == "round":
                L.append(f"        buf[{off}] = prim.round;")
            else:
                pidx = prim["_param_map"][fname]
                if ftype == "u32":
                    L.append(f"        std::memcpy(&buf[{off}], &prim.params[{pidx}], sizeof(float));")
                else:
                    L.append(f"        buf[{off}] = prim.params[{pidx}];")

        L.append(f"        return {prim['_word_count']};")
        L.append("    }")

    L.append("    default:")
    L.append("        return 0;")
    L.append("    }")
    L.append("}\n")

    # --- Compact GPU upload helpers ---
    L.append("/// Compute total bytes needed for compact prim buffer:")
    L.append("/// [offset_table: count words] + [compact_prim_data]")
    L.append("inline uint32_t computeCompactSize(")
    L.append("        const card::SDFPrimitive* prims, uint32_t count) {")
    L.append("    uint32_t dataWords = 0;")
    L.append("    float tmp[24];")
    L.append("    for (uint32_t i = 0; i < count; i++) {")
    L.append("        uint32_t wc = writePrimitive(tmp, prims[i]);")
    L.append("        dataWords += (wc > 0) ? wc : 1;")
    L.append("    }")
    L.append("    return (count + dataWords) * sizeof(float);")
    L.append("}\n")

    L.append("/// Write compact format to GPU buffer: [offset_table][compact_data].")
    L.append("/// Fills wordOffsets with per-prim word offsets (for grid translation).")
    L.append("inline void writeCompactToBuffer(")
    L.append("        float* buf, uint32_t bufBytes,")
    L.append("        const card::SDFPrimitive* prims, uint32_t count,")
    L.append("        std::vector<uint32_t>& wordOffsets) {")
    L.append("    wordOffsets.resize(count);")
    L.append("    float* dataBase = buf + count;")
    L.append("    uint32_t dataOffset = 0;")
    L.append("    for (uint32_t i = 0; i < count; i++) {")
    L.append("        wordOffsets[i] = dataOffset;")
    L.append("        uint32_t off = dataOffset;")
    L.append("        std::memcpy(&buf[i], &off, sizeof(uint32_t));")
    L.append("        uint32_t wc = writePrimitive(dataBase + dataOffset, prims[i]);")
    L.append("        if (wc == 0) wc = 1;")
    L.append("        dataOffset += wc;")
    L.append("    }")
    L.append("}\n")

    L.append("#endif // YETTY_CARD_SDF_PRIMITIVE_DEFINED\n")

    L.append("} // namespace yetty::sdf")
    L.append("")

    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(L))


# =============================================================================
# YDrawBuffer method generation — add/update per type
# =============================================================================

def generate_buffer(primitives: list[dict], out: Path) -> None:
    L = []
    L.append(HEADER)
    L.append("// Included inside YDrawBuffer class body.\n")

    for prim in primitives:
        fields = prim.get("fields", [])
        if not fields:
            continue

        name = prim["name"]
        wc = prim["_word_count"]
        writer_fn = "sdf::write" + name

        # Build parameter list (skip 'type' — implicit in writer)
        params = []
        param_names = []
        for field in fields:
            if field["name"] == "type":
                continue
            pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
            ctype = "uint32_t" if field["type"] == "u32" else "float"
            params.append(f"{ctype} {pname}")
            param_names.append(pname)

        param_str = ", ".join(params)
        arg_str = ", ".join(param_names)

        # --- add method: creates new prim, error if user id exists ---
        L.append(f"Result<uint32_t> add{name}({param_str},")
        L.append(f"        uint32_t id = AUTO_ID) {{")
        L.append(f"    float data[{wc}];")
        L.append(f"    {writer_fn}(data, {arg_str});")
        L.append(f"    return addPrim(id, data, {wc});")
        L.append("}\n")

        # --- update method: replaces existing, error if not found ---
        L.append(f"Result<void> update{name}(uint32_t id, {param_str}) {{")
        L.append(f"    float data[{wc}];")
        L.append(f"    {writer_fn}(data, {arg_str});")
        L.append(f"    return updatePrim(id, data, {wc});")
        L.append("}\n")

    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(L))


# =============================================================================
# C API generation — simple append-only buffer for FFI bindings
# =============================================================================

def generate_c_api(primitives: list[dict], out_h: Path, out_c: Path) -> None:
    """Generate C API header and implementation for YDraw buffer operations.

    Simple append-only buffer with auto-growth (like std::vector).
    No ID tracking - matches YGui's clear+rebuild pattern.
    """

    # Only generate for primitives with fields (skip stub-only 3D types)
    prims_with_fields = [p for p in primitives if p.get("fields")]

    # --- Generate Header ---
    H = []
    H.append(HEADER)
    H.append("""#ifndef YDRAW_CAPI_GEN_H
#define YDRAW_CAPI_GEN_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
 * YDraw C API - Generated from ydraw-primitives.yaml
 *
 * Simple append-only buffer for YDraw primitives.
 * Auto-grows when full (doubles capacity like std::vector).
 * Usage: create -> add primitives -> get data/size -> upload to GPU -> clear -> repeat
 *===========================================================================*/

/* Opaque buffer handle */
typedef struct ydraw_buffer ydraw_buffer_t;

/*=============================================================================
 * Buffer lifecycle
 *===========================================================================*/

/* Create a new YDraw buffer. Auto-grows as needed. */
ydraw_buffer_t* ydraw_buffer_create(void);

/* Destroy buffer and free all memory */
void ydraw_buffer_destroy(ydraw_buffer_t* buf);

/* Clear all primitives (resets to empty, keeps allocated memory) */
void ydraw_buffer_clear(ydraw_buffer_t* buf);

/* Get primitive count */
uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf);

/* Get word count (floats used) */
uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf);

/* Get byte size for GPU upload */
uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf);

/* Get raw data pointer for GPU upload (read-only) */
const float* ydraw_buffer_data(const ydraw_buffer_t* buf);

/* Serialize buffer to binary format for OSC transport.
 * Returns size in bytes, sets *out_data to point to internal buffer.
 * Data valid until next serialize() or destroy(). */
uint32_t ydraw_buffer_serialize(ydraw_buffer_t* buf, const uint8_t** out_data);

/*=============================================================================
 * Text spans - high-level text storage (converted to glyphs by builder)
 *===========================================================================*/

/* Text span data */
typedef struct {
    float x, y;
    const char* text;
    float fontSize;
    uint32_t color;
    uint32_t layer;
    int fontId;
    float rotation;
} ydraw_text_span_t;

/* Add text span (fontId -1 = default font) */
int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                       float fontSize, uint32_t color, uint32_t layer, int fontId);

/* Add rotated text span */
int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                               float fontSize, uint32_t color, float rotation, int fontId);

/* Get text span count */
uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf);

/* Get text span by index (returns NULL if out of bounds) */
const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index);

/*=============================================================================
 * Primitive type enum
 *===========================================================================*/

typedef enum {
""")

    for prim in primitives:
        H.append(f"    YDRAW_{camel_to_screaming_snake(prim['name'])} = {prim['id']},")

    H.append("} ydraw_prim_type_t;\n")

    H.append("""/*=============================================================================
 * Add primitive functions
 *
 * All functions append to buffer (auto-grows if needed).
 * Returns primitive index on success, -1 on failure (OOM).
 *===========================================================================*/
""")

    # Generate add function declarations
    for prim in prims_with_fields:
        fields = prim.get("fields", [])
        fn_name = f"ydraw_add_{camel_to_snake(prim['name'])}"

        # Build parameter list (skip 'type' - implicit)
        params = ["ydraw_buffer_t* buf"]
        for field in fields:
            if field["name"] == "type":
                continue
            pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
            ctype = "uint32_t" if field["type"] == "u32" else "float"
            params.append(f"{ctype} {pname}")

        param_str = ", ".join(params)
        H.append(f"int32_t {fn_name}({param_str});")

    H.append("")
    H.append("""#ifdef __cplusplus
}
#endif

#endif /* YDRAW_CAPI_GEN_H */
""")

    out_h.parent.mkdir(parents=True, exist_ok=True)
    out_h.write_text("\n".join(H))

    # --- Generate Implementation ---
    C = []
    C.append(HEADER)
    C.append("""#include "ydraw-capi.gen.h"
#include <stdlib.h>
#include <string.h>

/*=============================================================================
 * Buffer structure - simple auto-growing array
 *===========================================================================*/

#define YDRAW_INITIAL_CAPACITY 4096  /* Initial capacity in words (floats) */
#define YDRAW_INITIAL_TEXT_CAPACITY 64

/* Internal text span with owned string */
typedef struct {
    ydraw_text_span_t span;  /* Public data (text points to str_copy) */
    char* str_copy;          /* Owned copy of text string */
} ydraw_text_span_internal_t;

struct ydraw_buffer {
    /* Primitive data */
    float* data;
    uint32_t capacity;
    uint32_t used;
    uint32_t prim_count;

    /* Text spans */
    ydraw_text_span_internal_t* text_spans;
    uint32_t text_capacity;
    uint32_t text_count;

    /* Serialization buffer */
    uint8_t* serial_data;
    uint32_t serial_capacity;
    uint32_t serial_size;
};

/* Write uint32 to float buffer (bitcast) */
static inline void write_u32(float* buf, uint32_t off, uint32_t val) {
    memcpy(&buf[off], &val, sizeof(uint32_t));
}

/* Ensure buffer can hold additional words. Grows by doubling if needed.
 * Returns 1 on success, 0 on OOM. */
static int ensure_capacity(ydraw_buffer_t* buf, uint32_t additional_words) {
    uint32_t required = buf->used + additional_words;
    if (required <= buf->capacity) {
        return 1;  /* Already have space */
    }

    /* Double until we have enough */
    uint32_t new_cap = buf->capacity;
    while (new_cap < required) {
        new_cap *= 2;
    }

    float* new_data = (float*)realloc(buf->data, new_cap * sizeof(float));
    if (!new_data) {
        return 0;  /* OOM */
    }

    buf->data = new_data;
    buf->capacity = new_cap;
    return 1;
}

/*=============================================================================
 * Buffer lifecycle
 *===========================================================================*/

ydraw_buffer_t* ydraw_buffer_create(void) {
    ydraw_buffer_t* buf = (ydraw_buffer_t*)calloc(1, sizeof(ydraw_buffer_t));
    if (!buf) return NULL;

    buf->data = (float*)malloc(YDRAW_INITIAL_CAPACITY * sizeof(float));
    if (!buf->data) {
        free(buf);
        return NULL;
    }
    buf->capacity = YDRAW_INITIAL_CAPACITY;
    buf->used = 0;
    buf->prim_count = 0;

    buf->text_spans = (ydraw_text_span_internal_t*)malloc(
        YDRAW_INITIAL_TEXT_CAPACITY * sizeof(ydraw_text_span_internal_t));
    if (!buf->text_spans) {
        free(buf->data);
        free(buf);
        return NULL;
    }
    buf->text_capacity = YDRAW_INITIAL_TEXT_CAPACITY;
    buf->text_count = 0;

    return buf;
}

void ydraw_buffer_destroy(ydraw_buffer_t* buf) {
    if (!buf) return;
    /* Free text string copies */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        free(buf->text_spans[i].str_copy);
    }
    free(buf->text_spans);
    free(buf->data);
    free(buf);
}

void ydraw_buffer_clear(ydraw_buffer_t* buf) {
    if (!buf) return;
    buf->used = 0;
    buf->prim_count = 0;
    /* Free text string copies */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        free(buf->text_spans[i].str_copy);
    }
    buf->text_count = 0;
    /* Keep allocated memory for reuse */
}

uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf) {
    return buf ? buf->prim_count : 0;
}

uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf) {
    return buf ? buf->used : 0;
}

uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf) {
    return buf ? buf->used * sizeof(float) : 0;
}

const float* ydraw_buffer_data(const ydraw_buffer_t* buf) {
    return buf ? buf->data : NULL;
}

/*=============================================================================
 * Serialization - compatible with C++ YDrawBuffer::serialize()
 *===========================================================================*/

#define YDRAW_SERIALIZE_MAGIC   0x59445246  /* "YDRF" */
#define YDRAW_SERIALIZE_VERSION 1

uint32_t ydraw_buffer_serialize(ydraw_buffer_t* buf, const uint8_t** out_data) {
    if (!buf || !out_data) {
        if (out_data) *out_data = NULL;
        return 0;
    }

    /* Calculate required size */
    size_t size = 8;  /* magic + version */
    size += 8;  /* primCount + totalWords */
    size += buf->used * sizeof(float);  /* prim data */
    size += 4;  /* fontCount (always 0 for now) */
    size += 4;  /* spanCount */
    for (uint32_t i = 0; i < buf->text_count; i++) {
        size += 4 * 7;  /* x, y, fontSize, color, layer, fontId, rotation */
        size += 4;  /* textLen */
        size += strlen(buf->text_spans[i].span.text);
    }
    size += 4 + 4 + 1 + 16;  /* scene metadata */

    /* Allocate/reallocate serialization buffer */
    if (buf->serial_capacity < size) {
        uint8_t* new_buf = (uint8_t*)realloc(buf->serial_data, size);
        if (!new_buf) {
            *out_data = NULL;
            return 0;
        }
        buf->serial_data = new_buf;
        buf->serial_capacity = (uint32_t)size;
    }

    uint8_t* p = buf->serial_data;

    /* Magic & version */
    memcpy(p, &(uint32_t){YDRAW_SERIALIZE_MAGIC}, 4); p += 4;
    memcpy(p, &(uint32_t){YDRAW_SERIALIZE_VERSION}, 4); p += 4;

    /* Primitives */
    memcpy(p, &buf->prim_count, 4); p += 4;
    memcpy(p, &buf->used, 4); p += 4;
    if (buf->used > 0) {
        memcpy(p, buf->data, buf->used * sizeof(float));
        p += buf->used * sizeof(float);
    }

    /* Fonts (none for now) */
    memcpy(p, &(uint32_t){0}, 4); p += 4;

    /* Text spans */
    memcpy(p, &buf->text_count, 4); p += 4;
    for (uint32_t i = 0; i < buf->text_count; i++) {
        ydraw_text_span_t* ts = &buf->text_spans[i].span;
        memcpy(p, &ts->x, 4); p += 4;
        memcpy(p, &ts->y, 4); p += 4;
        memcpy(p, &ts->fontSize, 4); p += 4;
        memcpy(p, &ts->color, 4); p += 4;
        memcpy(p, &ts->layer, 4); p += 4;
        int32_t fontId = ts->fontId;
        memcpy(p, &fontId, 4); p += 4;
        memcpy(p, &ts->rotation, 4); p += 4;
        uint32_t textLen = (uint32_t)strlen(ts->text);
        memcpy(p, &textLen, 4); p += 4;
        memcpy(p, ts->text, textLen); p += textLen;
    }

    /* Scene metadata (defaults) */
    memcpy(p, &(uint32_t){0}, 4); p += 4;  /* bgColor */
    memcpy(p, &(uint32_t){0}, 4); p += 4;  /* flags */
    *p++ = 0;  /* hasSceneBounds */
    memcpy(p, &(float){0}, 4); p += 4;  /* minX */
    memcpy(p, &(float){0}, 4); p += 4;  /* minY */
    memcpy(p, &(float){0}, 4); p += 4;  /* maxX */
    memcpy(p, &(float){0}, 4); p += 4;  /* maxY */

    buf->serial_size = (uint32_t)(p - buf->serial_data);
    *out_data = buf->serial_data;
    return buf->serial_size;
}

/*=============================================================================
 * Text span functions
 *===========================================================================*/

static int ensure_text_capacity(ydraw_buffer_t* buf) {
    if (buf->text_count < buf->text_capacity) return 1;

    uint32_t new_cap = buf->text_capacity * 2;
    ydraw_text_span_internal_t* new_spans = (ydraw_text_span_internal_t*)realloc(
        buf->text_spans, new_cap * sizeof(ydraw_text_span_internal_t));
    if (!new_spans) return 0;

    buf->text_spans = new_spans;
    buf->text_capacity = new_cap;
    return 1;
}

int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                       float fontSize, uint32_t color, uint32_t layer, int fontId) {
    if (!buf || !text) return -1;
    if (!ensure_text_capacity(buf)) return -1;

    size_t len = strlen(text);
    char* str_copy = (char*)malloc(len + 1);
    if (!str_copy) return -1;
    memcpy(str_copy, text, len + 1);

    ydraw_text_span_internal_t* ts = &buf->text_spans[buf->text_count];
    ts->str_copy = str_copy;
    ts->span.x = x;
    ts->span.y = y;
    ts->span.text = str_copy;
    ts->span.fontSize = fontSize;
    ts->span.color = color;
    ts->span.layer = layer;
    ts->span.fontId = fontId;
    ts->span.rotation = 0.0f;

    return (int32_t)buf->text_count++;
}

int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                               float fontSize, uint32_t color, float rotation, int fontId) {
    if (!buf || !text) return -1;
    if (!ensure_text_capacity(buf)) return -1;

    size_t len = strlen(text);
    char* str_copy = (char*)malloc(len + 1);
    if (!str_copy) return -1;
    memcpy(str_copy, text, len + 1);

    ydraw_text_span_internal_t* ts = &buf->text_spans[buf->text_count];
    ts->str_copy = str_copy;
    ts->span.x = x;
    ts->span.y = y;
    ts->span.text = str_copy;
    ts->span.fontSize = fontSize;
    ts->span.color = color;
    ts->span.layer = 0;
    ts->span.fontId = fontId;
    ts->span.rotation = rotation;

    return (int32_t)buf->text_count++;
}

uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf) {
    return buf ? buf->text_count : 0;
}

const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index) {
    if (!buf || index >= buf->text_count) return NULL;
    return &buf->text_spans[index].span;
}

/*=============================================================================
 * Add primitive functions
 *===========================================================================*/
""")

    # Generate add function implementations
    for prim in prims_with_fields:
        fields = prim.get("fields", [])
        wc = prim["_word_count"]
        fn_name = f"ydraw_add_{camel_to_snake(prim['name'])}"

        # Build parameter list
        params = ["ydraw_buffer_t* buf"]
        for field in fields:
            if field["name"] == "type":
                continue
            pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
            ctype = "uint32_t" if field["type"] == "u32" else "float"
            params.append(f"{ctype} {pname}")

        param_str = ", ".join(params)

        C.append(f"int32_t {fn_name}({param_str}) {{")
        C.append(f"    if (!buf) return -1;")
        C.append(f"    if (!ensure_capacity(buf, {wc})) return -1;")
        C.append(f"")
        C.append(f"    float* p = buf->data + buf->used;")

        # Write each field
        for field in fields:
            off = prim["_offset_map"][field["name"]][0]
            if field["name"] == "type":
                C.append(f"    write_u32(p, {off}, {prim['id']}u);")
            else:
                pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
                if field["type"] == "u32":
                    C.append(f"    write_u32(p, {off}, {pname});")
                else:
                    C.append(f"    p[{off}] = {pname};")

        C.append(f"")
        C.append(f"    buf->used += {wc};")
        C.append(f"    return (int32_t)buf->prim_count++;")
        C.append(f"}}")
        C.append(f"")

    out_c.parent.mkdir(parents=True, exist_ok=True)
    out_c.write_text("\n".join(C))


# =============================================================================
# C++ Wrapper generation — wraps YDrawBuffer for FFI use
# =============================================================================

def generate_c_api_wrapper(primitives: list[dict], out: Path) -> None:
    """Generate C++ wrapper that delegates to YDrawBuffer.

    This wraps the C++ YDrawBuffer class with C linkage functions,
    avoiding maintaining two separate implementations.
    """

    prims_with_fields = [p for p in primitives if p.get("fields")]

    W = []
    W.append(HEADER)
    W.append("""
#include "ydraw-capi.gen.h"
#include "ydraw-buffer.h"

#include <cstring>
#include <cstdlib>

using namespace yetty;

//=============================================================================
// The opaque ydraw_buffer_t wraps YDrawBuffer::Ptr
//=============================================================================

struct ydraw_buffer {
    YDrawBuffer::Ptr impl;
    std::vector<uint8_t> serialized;  // Cached for C API access
};

//=============================================================================
// Buffer lifecycle
//=============================================================================

extern "C" ydraw_buffer_t* ydraw_buffer_create(void) {
    auto result = YDrawBuffer::create();
    if (!result) return nullptr;
    auto* buf = new ydraw_buffer();
    buf->impl = *result;
    return buf;
}

extern "C" void ydraw_buffer_destroy(ydraw_buffer_t* buf) {
    delete buf;
}

extern "C" void ydraw_buffer_clear(ydraw_buffer_t* buf) {
    if (buf && buf->impl) {
        buf->impl->clear();
        buf->serialized.clear();
    }
}

extern "C" uint32_t ydraw_buffer_prim_count(const ydraw_buffer_t* buf) {
    return (buf && buf->impl) ? buf->impl->primCount() : 0;
}

extern "C" uint32_t ydraw_buffer_word_count(const ydraw_buffer_t* buf) {
    return 0;  // Not directly available
}

extern "C" uint32_t ydraw_buffer_byte_size(const ydraw_buffer_t* buf) {
    return buf ? static_cast<uint32_t>(buf->serialized.size()) : 0;
}

extern "C" const float* ydraw_buffer_data(const ydraw_buffer_t* buf) {
    return nullptr;  // Not directly exposed
}

extern "C" uint32_t ydraw_buffer_serialize(ydraw_buffer_t* buf, const uint8_t** out_data) {
    if (!buf || !buf->impl || !out_data) {
        if (out_data) *out_data = nullptr;
        return 0;
    }
    buf->serialized = buf->impl->serialize();
    *out_data = buf->serialized.data();
    return static_cast<uint32_t>(buf->serialized.size());
}

//=============================================================================
// Text spans
//=============================================================================

extern "C" int32_t ydraw_add_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                                   float fontSize, uint32_t color, uint32_t layer, int fontId) {
    if (!buf || !buf->impl || !text) return -1;
    buf->impl->addTextSpan(x, y, text, fontSize, color, layer, fontId);
    return 0;
}

extern "C" int32_t ydraw_add_rotated_text(ydraw_buffer_t* buf, float x, float y, const char* text,
                                           float fontSize, uint32_t color, float rotation, int fontId) {
    if (!buf || !buf->impl || !text) return -1;
    buf->impl->addRotatedTextSpan(x, y, text, fontSize, color, rotation, fontId);
    return 0;
}

extern "C" uint32_t ydraw_buffer_text_span_count(const ydraw_buffer_t* buf) {
    return (buf && buf->impl) ? static_cast<uint32_t>(buf->impl->textSpans().size()) : 0;
}

extern "C" const ydraw_text_span_t* ydraw_buffer_get_text_span(const ydraw_buffer_t* buf, uint32_t index) {
    return nullptr;  // Would need caching
}

//=============================================================================
// Primitive add functions - delegate to YDrawBuffer
//=============================================================================
""")

    # Generate wrapper for each primitive
    for prim in prims_with_fields:
        fields = prim.get("fields", [])
        fn_name = f"ydraw_add_{camel_to_snake(prim['name'])}"
        cpp_method = f"add{prim['name']}"

        # Build C parameter list
        params = ["ydraw_buffer_t* buf"]
        args = []  # Arguments to pass to C++ method

        for field in fields:
            if field["name"] == "type":
                continue
            pname = field["name"] + "_" if field["name"] in ("round",) else field["name"]
            ctype = "uint32_t" if field["type"] == "u32" else "float"
            params.append(f"{ctype} {pname}")
            args.append(pname)

        param_str = ", ".join(params)
        args_str = ", ".join(args)

        W.append(f"extern \"C\" int32_t {fn_name}({param_str}) {{")
        W.append(f"    if (!buf || !buf->impl) return -1;")
        W.append(f"    auto result = buf->impl->{cpp_method}({args_str});")
        W.append(f"    return result ? static_cast<int32_t>(*result) : -1;")
        W.append(f"}}")
        W.append("")

    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(W))


# =============================================================================
# Main
# =============================================================================

def main() -> None:
    if not YAML_PATH.exists():
        print(f"Error: {YAML_PATH} not found", file=sys.stderr)
        sys.exit(1)

    primitives = load_primitives(YAML_PATH)

    generate_cpp(primitives, CPP_OUT)
    generate_wgsl(primitives, WGSL_OUT)
    generate_writer(primitives, WRITER_OUT)
    generate_buffer(primitives, BUFFER_OUT)
    generate_c_api(primitives, CAPI_H_OUT, CAPI_C_OUT)
    generate_c_api_wrapper(primitives, CAPI_WRAPPER_OUT)

    # Summary
    cats = {}
    for p in primitives:
        cats.setdefault(p["category"], []).append(p["name"])
    print(f"Read {len(primitives)} primitives from {YAML_PATH.name}:")
    for cat, names in cats.items():
        print(f"  {cat}: {len(names)}")
    print(f"Generated {CPP_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {WGSL_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {WRITER_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {BUFFER_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {CAPI_H_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {CAPI_C_OUT.relative_to(PROJECT_ROOT)}")
    print(f"Generated {CAPI_WRAPPER_OUT.relative_to(PROJECT_ROOT)}")


if __name__ == "__main__":
    main()
