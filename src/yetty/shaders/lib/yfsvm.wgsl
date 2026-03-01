// yfsvm - GPU Bytecode Interpreter
// AUTO-GENERATED from yfsvm opcode definitions - DO NOT EDIT MANUALLY
//
// This interpreter executes yfsvm bytecode in the fragment shader.
// Bytecode is stored in cardStorage buffer, constants inline in the program.

// Constants
const YFSVM_MAGIC: u32 = 0x5946534du;
const YFSVM_VERSION: u32 = 1u;
const YFSVM_MAX_REGISTERS: u32 = 16u;
const YFSVM_MAX_FUNCTIONS: u32 = 16u;

// Opcodes
const OP_NOP: u32 = 0x0u;
const OP_RET: u32 = 0x1u;
const OP_LOAD_C: u32 = 0x2u;
const OP_LOAD_X: u32 = 0x3u;
const OP_LOAD_T: u32 = 0x4u;
const OP_LOAD_S: u32 = 0x5u;
const OP_MOV: u32 = 0x6u;
const OP_ADD: u32 = 0x10u;
const OP_SUB: u32 = 0x11u;
const OP_MUL: u32 = 0x12u;
const OP_DIV: u32 = 0x13u;
const OP_NEG: u32 = 0x14u;
const OP_MOD: u32 = 0x15u;
const OP_SIN: u32 = 0x20u;
const OP_COS: u32 = 0x21u;
const OP_TAN: u32 = 0x22u;
const OP_ASIN: u32 = 0x23u;
const OP_ACOS: u32 = 0x24u;
const OP_ATAN: u32 = 0x25u;
const OP_ATAN2: u32 = 0x26u;
const OP_SINH: u32 = 0x27u;
const OP_COSH: u32 = 0x28u;
const OP_TANH: u32 = 0x29u;
const OP_EXP: u32 = 0x30u;
const OP_EXP2: u32 = 0x31u;
const OP_LOG: u32 = 0x32u;
const OP_LOG2: u32 = 0x33u;
const OP_POW: u32 = 0x34u;
const OP_SQRT: u32 = 0x35u;
const OP_RSQRT: u32 = 0x36u;
const OP_ABS: u32 = 0x40u;
const OP_MIN: u32 = 0x41u;
const OP_MAX: u32 = 0x42u;
const OP_FLOOR: u32 = 0x43u;
const OP_CEIL: u32 = 0x44u;
const OP_ROUND: u32 = 0x45u;
const OP_FRACT: u32 = 0x46u;
const OP_SIGN: u32 = 0x47u;
const OP_CLAMP01: u32 = 0x48u;
const OP_STEP: u32 = 0x49u;
const OP_MIX: u32 = 0x4au;

// Instruction decoding (matches C++ Instruction struct)
fn yfsvm_decode_opcode(instr: u32) -> u32 { return (instr >> 24u) & 0xFFu; }
fn yfsvm_decode_dst(instr: u32) -> u32 { return (instr >> 20u) & 0xFu; }
fn yfsvm_decode_src1(instr: u32) -> u32 { return (instr >> 16u) & 0xFu; }
fn yfsvm_decode_src2(instr: u32) -> u32 { return (instr >> 12u) & 0xFu; }
fn yfsvm_decode_imm12(instr: u32) -> u32 { return instr & 0xFFFu; }

// Register read/write helpers
fn yfsvm_read_reg(regs: ptr<function, array<f32, 16>>, idx: u32) -> f32 {
    // Manual switch because WGSL doesn't support dynamic array indexing in all cases
    switch idx {
        case 0u: { return (*regs)[0]; }
        case 1u: { return (*regs)[1]; }
        case 2u: { return (*regs)[2]; }
        case 3u: { return (*regs)[3]; }
        case 4u: { return (*regs)[4]; }
        case 5u: { return (*regs)[5]; }
        case 6u: { return (*regs)[6]; }
        case 7u: { return (*regs)[7]; }
        case 8u: { return (*regs)[8]; }
        case 9u: { return (*regs)[9]; }
        case 10u: { return (*regs)[10]; }
        case 11u: { return (*regs)[11]; }
        case 12u: { return (*regs)[12]; }
        case 13u: { return (*regs)[13]; }
        case 14u: { return (*regs)[14]; }
        case 15u: { return (*regs)[15]; }
        default: { return 0.0; }
    }
}

fn yfsvm_write_reg(regs: ptr<function, array<f32, 16>>, idx: u32, val: f32) {
    switch idx {
        case 0u: { (*regs)[0] = val; }
        case 1u: { (*regs)[1] = val; }
        case 2u: { (*regs)[2] = val; }
        case 3u: { (*regs)[3] = val; }
        case 4u: { (*regs)[4] = val; }
        case 5u: { (*regs)[5] = val; }
        case 6u: { (*regs)[6] = val; }
        case 7u: { (*regs)[7] = val; }
        case 8u: { (*regs)[8] = val; }
        case 9u: { (*regs)[9] = val; }
        case 10u: { (*regs)[10] = val; }
        case 11u: { (*regs)[11] = val; }
        case 12u: { (*regs)[12] = val; }
        case 13u: { (*regs)[13] = val; }
        case 14u: { (*regs)[14] = val; }
        case 15u: { (*regs)[15] = val; }
        default: {}
    }
}

// Execute yfsvm bytecode for a single function
// bytecodeOffset: index into cardStorage where program starts
// funcIndex: which function to execute (0-based)
// x: domain value (the x coordinate)
// t: time value
// samplers: additional input values (s0-s7)
fn yfsvm_execute(bytecodeOffset: u32, funcIndex: u32, x: f32, t: f32, samplers: array<f32, 8>) -> f32 {
    // Read program header
    let magic = cardStorage[bytecodeOffset];
    if (magic != YFSVM_MAGIC) {
        return 0.0; // Invalid program
    }

    let version = cardStorage[bytecodeOffset + 1u];
    let funcCount = cardStorage[bytecodeOffset + 2u];
    let constCount = cardStorage[bytecodeOffset + 3u];

    if (funcIndex >= funcCount) {
        return 0.0; // Function index out of range
    }

    // Function table starts at offset 4
    let funcTableOffset = bytecodeOffset + 4u;
    let funcOffset = cardStorage[funcTableOffset + funcIndex * 2u];
    let funcLength = cardStorage[funcTableOffset + funcIndex * 2u + 1u];

    // Constants start after function table
    let constOffset = funcTableOffset + funcCount * 2u;

    // Code starts after constants
    let codeOffset = constOffset + constCount;

    // Initialize registers
    var regs: array<f32, 16>;
    for (var i = 0u; i < 16u; i = i + 1u) {
        regs[i] = 0.0;
    }

    // Execute instructions
    var pc = funcOffset;
    let endPc = funcOffset + funcLength;

    for (var iter = 0u; iter < 256u; iter = iter + 1u) {
        if (pc >= endPc) {
            break;
        }

        let instr = cardStorage[codeOffset + pc];
        pc = pc + 1u;

        let op = yfsvm_decode_opcode(instr);
        let dst = yfsvm_decode_dst(instr);
        let src1 = yfsvm_decode_src1(instr);
        let src2 = yfsvm_decode_src2(instr);
        let imm = yfsvm_decode_imm12(instr);

        // Read source registers
        let v1 = yfsvm_read_reg(&regs, src1);
        let v2 = yfsvm_read_reg(&regs, src2);

        switch op {
            case OP_NOP: {}
            case OP_RET: { return regs[0]; }
            case OP_LOAD_C: {
                let c = bitcast<f32>(cardStorage[constOffset + imm]);
                yfsvm_write_reg(&regs, dst, c);
            }
            case OP_LOAD_X: { yfsvm_write_reg(&regs, dst, x); }
            case OP_LOAD_T: { yfsvm_write_reg(&regs, dst, t); }
            case OP_LOAD_S: {
                let si = imm & 7u;
                var sv = 0.0;
                switch si {
                    case 0u: { sv = samplers[0]; }
                    case 1u: { sv = samplers[1]; }
                    case 2u: { sv = samplers[2]; }
                    case 3u: { sv = samplers[3]; }
                    case 4u: { sv = samplers[4]; }
                    case 5u: { sv = samplers[5]; }
                    case 6u: { sv = samplers[6]; }
                    case 7u: { sv = samplers[7]; }
                    default: {}
                }
                yfsvm_write_reg(&regs, dst, sv);
            }
            case OP_MOV: { yfsvm_write_reg(&regs, dst, v1); }

            // Arithmetic
            case OP_ADD: { yfsvm_write_reg(&regs, dst, v1 + v2); }
            case OP_SUB: { yfsvm_write_reg(&regs, dst, v1 - v2); }
            case OP_MUL: { yfsvm_write_reg(&regs, dst, v1 * v2); }
            case OP_DIV: { yfsvm_write_reg(&regs, dst, v1 / v2); }
            case OP_NEG: { yfsvm_write_reg(&regs, dst, -v1); }
            case OP_MOD: { yfsvm_write_reg(&regs, dst, v1 % v2); }

            // Transcendentals
            case OP_SIN: { yfsvm_write_reg(&regs, dst, sin(v1)); }
            case OP_COS: { yfsvm_write_reg(&regs, dst, cos(v1)); }
            case OP_TAN: { yfsvm_write_reg(&regs, dst, tan(v1)); }
            case OP_ASIN: { yfsvm_write_reg(&regs, dst, asin(v1)); }
            case OP_ACOS: { yfsvm_write_reg(&regs, dst, acos(v1)); }
            case OP_ATAN: { yfsvm_write_reg(&regs, dst, atan(v1)); }
            case OP_ATAN2: { yfsvm_write_reg(&regs, dst, atan2(v1, v2)); }
            case OP_SINH: { yfsvm_write_reg(&regs, dst, sinh(v1)); }
            case OP_COSH: { yfsvm_write_reg(&regs, dst, cosh(v1)); }
            case OP_TANH: { yfsvm_write_reg(&regs, dst, tanh(v1)); }

            // Exponential/Logarithmic
            case OP_EXP: { yfsvm_write_reg(&regs, dst, exp(v1)); }
            case OP_EXP2: { yfsvm_write_reg(&regs, dst, exp2(v1)); }
            case OP_LOG: { yfsvm_write_reg(&regs, dst, log(v1)); }
            case OP_LOG2: { yfsvm_write_reg(&regs, dst, log2(v1)); }
            case OP_POW: { yfsvm_write_reg(&regs, dst, pow(v1, v2)); }
            case OP_SQRT: { yfsvm_write_reg(&regs, dst, sqrt(v1)); }
            case OP_RSQRT: { yfsvm_write_reg(&regs, dst, inverseSqrt(v1)); }

            // Utility
            case OP_ABS: { yfsvm_write_reg(&regs, dst, abs(v1)); }
            case OP_MIN: { yfsvm_write_reg(&regs, dst, min(v1, v2)); }
            case OP_MAX: { yfsvm_write_reg(&regs, dst, max(v1, v2)); }
            case OP_FLOOR: { yfsvm_write_reg(&regs, dst, floor(v1)); }
            case OP_CEIL: { yfsvm_write_reg(&regs, dst, ceil(v1)); }
            case OP_ROUND: { yfsvm_write_reg(&regs, dst, round(v1)); }
            case OP_FRACT: { yfsvm_write_reg(&regs, dst, fract(v1)); }
            case OP_SIGN: { yfsvm_write_reg(&regs, dst, sign(v1)); }
            case OP_CLAMP01: { yfsvm_write_reg(&regs, dst, clamp(v1, 0.0, 1.0)); }
            case OP_STEP: { yfsvm_write_reg(&regs, dst, step(v1, v2)); }
            case OP_MIX: {
                let tval = yfsvm_read_reg(&regs, imm & 0xFu);
                yfsvm_write_reg(&regs, dst, mix(v1, v2, tval));
            }

            default: {}
        }
    }

    return regs[0];
}
