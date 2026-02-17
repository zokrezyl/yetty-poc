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

    # --- evalSDF (2D) — compact layout dispatch ---
    sdf2d = [p for p in primitives if p["category"] == "sdf2d"]
    if sdf2d:
        L.append("fn evalSDF(primOffset: u32, p: vec2<f32>) -> f32 {")
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

    # --- evalSDF3D ---
    sdf3d = [p for p in primitives if p["category"] == "sdf3d"]
    if sdf3d:
        L.append("fn evalSDF3D(primOffset: u32, p: vec3<f32>) -> f32 {")
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
    renderable = [
        p for p in primitives
        if "fillColor" in p["_offset_map"]
        and "strokeColor" in p["_offset_map"]
        and "layer" in p["_offset_map"]
    ]
    if renderable:
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
        L.append("        default: { return vec4<u32>(0u); }")
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


if __name__ == "__main__":
    main()
