// yfsvm.wgsl - Yetty Fragment Shader Virtual Machine
//
// A register-based bytecode interpreter for mathematical expressions.
// Eliminates shader recompilation when plot expressions change.
//
// Bytecode buffer layout:
//   [0]     magic (0x5946534D = "YFSM")
//   [1]     version
//   [2]     function count
//   [3]     constant count
//   [4..19] function table (16 entries): packed(length:16, offset:16)
//   [20..]  constants (f32 as u32)
//   [..]    code
//
// Instruction format (32-bit):
//   [31:24] opcode
//   [23:20] dst register
//   [19:16] src1 register
//   [15:12] src2 register
//   [11:0]  imm12 (constant index or extra data)

// Opcodes
const OP_NOP: u32      = 0x00u;
const OP_RET: u32      = 0x01u;
const OP_LOAD_C: u32   = 0x02u;
const OP_LOAD_X: u32   = 0x03u;
const OP_LOAD_T: u32   = 0x04u;
const OP_LOAD_S: u32   = 0x05u;
const OP_MOV: u32      = 0x06u;
const OP_ADD: u32      = 0x10u;
const OP_SUB: u32      = 0x11u;
const OP_MUL: u32      = 0x12u;
const OP_DIV: u32      = 0x13u;
const OP_NEG: u32      = 0x14u;
const OP_MOD: u32      = 0x15u;
const OP_SIN: u32      = 0x20u;
const OP_COS: u32      = 0x21u;
const OP_TAN: u32      = 0x22u;
const OP_ASIN: u32     = 0x23u;
const OP_ACOS: u32     = 0x24u;
const OP_ATAN: u32     = 0x25u;
const OP_ATAN2: u32    = 0x26u;
const OP_SINH: u32     = 0x27u;
const OP_COSH: u32     = 0x28u;
const OP_TANH: u32     = 0x29u;
const OP_EXP: u32      = 0x30u;
const OP_EXP2: u32     = 0x31u;
const OP_LOG: u32      = 0x32u;
const OP_LOG2: u32     = 0x33u;
const OP_POW: u32      = 0x34u;
const OP_SQRT: u32     = 0x35u;
const OP_RSQRT: u32    = 0x36u;
const OP_ABS: u32      = 0x40u;
const OP_MIN: u32      = 0x41u;
const OP_MAX: u32      = 0x42u;
const OP_FLOOR: u32    = 0x43u;
const OP_CEIL: u32     = 0x44u;
const OP_ROUND: u32    = 0x45u;
const OP_FRACT: u32    = 0x46u;
const OP_SIGN: u32     = 0x47u;
const OP_CLAMP01: u32  = 0x48u;
const OP_STEP: u32     = 0x49u;
const OP_MIX: u32      = 0x4Au;

// VM header offsets
const YFSVM_HEADER_SIZE: u32 = 4u;
const YFSVM_FUNC_TABLE_SIZE: u32 = 16u;
const YFSVM_MAX_INSTRUCTIONS: u32 = 256u;

// Decode instruction fields
fn yfsvm_opcode(instr: u32) -> u32 {
    return (instr >> 24u) & 0xFFu;
}

fn yfsvm_dst(instr: u32) -> u32 {
    return (instr >> 20u) & 0xFu;
}

fn yfsvm_src1(instr: u32) -> u32 {
    return (instr >> 16u) & 0xFu;
}

fn yfsvm_src2(instr: u32) -> u32 {
    return (instr >> 12u) & 0xFu;
}

fn yfsvm_imm12(instr: u32) -> u32 {
    return instr & 0xFFFu;
}

// Execute a yfsvm program
// bufferOffset: offset into cardStorage where bytecode starts
// funcIndex: which function to execute (0-15)
// x: domain value
// t: time
// samplers: array of 8 sampler values
fn yfsvm_execute(bufferOffset: u32, funcIndex: u32, x: f32, t: f32, samplers: array<f32, 8>) -> f32 {
    // Read header
    let funcCount = cardStorage[bufferOffset + 2u];
    let constCount = cardStorage[bufferOffset + 3u];

    if (funcIndex >= funcCount) {
        return 0.0;
    }

    // Read function entry from table
    let funcEntry = bitcast<u32>(cardStorage[bufferOffset + YFSVM_HEADER_SIZE + funcIndex]);
    let codeOffset = funcEntry & 0xFFFFu;
    let codeLength = funcEntry >> 16u;

    // Calculate absolute offsets
    let constBase = bufferOffset + YFSVM_HEADER_SIZE + YFSVM_FUNC_TABLE_SIZE;
    let codeBase = constBase + constCount;

    // Registers (16 general purpose)
    var regs: array<f32, 16>;

    // Execute instructions
    var pc: u32 = 0u;
    for (var i = 0u; i < YFSVM_MAX_INSTRUCTIONS; i = i + 1u) {
        if (pc >= codeLength) {
            break;
        }

        let instr = bitcast<u32>(cardStorage[codeBase + codeOffset + pc]);
        pc = pc + 1u;

        let op = yfsvm_opcode(instr);
        let dst = yfsvm_dst(instr);
        let src1 = yfsvm_src1(instr);
        let src2 = yfsvm_src2(instr);
        let imm = yfsvm_imm12(instr);

        switch (op) {
            case OP_NOP: {
                // Do nothing
            }
            case OP_RET: {
                return regs[0];
            }
            case OP_LOAD_C: {
                regs[dst] = bitcast<f32>(cardStorage[constBase + imm]);
            }
            case OP_LOAD_X: {
                regs[dst] = x;
            }
            case OP_LOAD_T: {
                regs[dst] = t;
            }
            case OP_LOAD_S: {
                let sidx = imm & 0x7u;
                regs[dst] = samplers[sidx];
            }
            case OP_MOV: {
                regs[dst] = regs[src1];
            }
            case OP_ADD: {
                regs[dst] = regs[src1] + regs[src2];
            }
            case OP_SUB: {
                regs[dst] = regs[src1] - regs[src2];
            }
            case OP_MUL: {
                regs[dst] = regs[src1] * regs[src2];
            }
            case OP_DIV: {
                regs[dst] = regs[src1] / regs[src2];
            }
            case OP_NEG: {
                regs[dst] = -regs[src1];
            }
            case OP_MOD: {
                regs[dst] = regs[src1] % regs[src2];
            }
            case OP_SIN: {
                regs[dst] = sin(regs[src1]);
            }
            case OP_COS: {
                regs[dst] = cos(regs[src1]);
            }
            case OP_TAN: {
                regs[dst] = tan(regs[src1]);
            }
            case OP_ASIN: {
                regs[dst] = asin(regs[src1]);
            }
            case OP_ACOS: {
                regs[dst] = acos(regs[src1]);
            }
            case OP_ATAN: {
                regs[dst] = atan(regs[src1]);
            }
            case OP_ATAN2: {
                regs[dst] = atan2(regs[src1], regs[src2]);
            }
            case OP_SINH: {
                regs[dst] = sinh(regs[src1]);
            }
            case OP_COSH: {
                regs[dst] = cosh(regs[src1]);
            }
            case OP_TANH: {
                regs[dst] = tanh(regs[src1]);
            }
            case OP_EXP: {
                regs[dst] = exp(regs[src1]);
            }
            case OP_EXP2: {
                regs[dst] = exp2(regs[src1]);
            }
            case OP_LOG: {
                regs[dst] = log(regs[src1]);
            }
            case OP_LOG2: {
                regs[dst] = log2(regs[src1]);
            }
            case OP_POW: {
                regs[dst] = pow(regs[src1], regs[src2]);
            }
            case OP_SQRT: {
                regs[dst] = sqrt(regs[src1]);
            }
            case OP_RSQRT: {
                regs[dst] = inverseSqrt(regs[src1]);
            }
            case OP_ABS: {
                regs[dst] = abs(regs[src1]);
            }
            case OP_MIN: {
                regs[dst] = min(regs[src1], regs[src2]);
            }
            case OP_MAX: {
                regs[dst] = max(regs[src1], regs[src2]);
            }
            case OP_FLOOR: {
                regs[dst] = floor(regs[src1]);
            }
            case OP_CEIL: {
                regs[dst] = ceil(regs[src1]);
            }
            case OP_ROUND: {
                regs[dst] = round(regs[src1]);
            }
            case OP_FRACT: {
                regs[dst] = fract(regs[src1]);
            }
            case OP_SIGN: {
                regs[dst] = sign(regs[src1]);
            }
            case OP_CLAMP01: {
                regs[dst] = clamp(regs[src1], 0.0, 1.0);
            }
            case OP_STEP: {
                regs[dst] = step(regs[src1], regs[src2]);
            }
            case OP_MIX: {
                let tReg = imm & 0xFu;
                regs[dst] = mix(regs[src1], regs[src2], regs[tReg]);
            }
            default: {
                // Unknown opcode, stop execution
                return regs[0];
            }
        }
    }

    // Implicit return r0
    return regs[0];
}

// Convenience wrapper for single-function programs with default samplers
fn yfsvm_eval(bufferOffset: u32, x: f32, t: f32) -> f32 {
    var samplers: array<f32, 8>;
    samplers[0] = x;  // s0 = x by default
    samplers[1] = t;  // s1 = t by default
    return yfsvm_execute(bufferOffset, 0u, x, t, samplers);
}
