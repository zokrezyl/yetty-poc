#pragma once

// yfsvm - Yetty Fragment Shader Virtual Machine
//
// A register-based bytecode VM that runs in the fragment shader.
// Eliminates shader recompilation when plot expressions change.
//
// Design:
//   - 16 general-purpose registers (r0-r15)
//   - r0 is the return value
//   - 32-bit instruction encoding
//   - Constant pool for float literals
//   - Input mapping: x, t, s0-s7 (samplers)
//
// Instruction format (32-bit):
//   [31:24] opcode (8 bits)
//   [23:20] dst    (4 bits) - destination register
//   [19:16] src1   (4 bits) - first source register
//   [15:12] src2   (4 bits) - second source register
//   [11:0]  imm12  (12 bits) - immediate/constant index

#include <yetty/result.hpp>
#include <cstdint>
#include <vector>
#include <string>
#include <span>

namespace yetty::yfsvm {

//=============================================================================
// Constants
//=============================================================================

constexpr uint32_t YFSVM_MAGIC = 0x5946534D;  // "YFSM"
constexpr uint32_t YFSVM_VERSION = 1;
constexpr uint32_t YFSVM_MAX_REGISTERS = 16;
constexpr uint32_t YFSVM_MAX_FUNCTIONS = 16;
constexpr uint32_t YFSVM_MAX_CONSTANTS = 4096;
constexpr uint32_t YFSVM_MAX_INSTRUCTIONS = 256;

//=============================================================================
// Opcodes
//=============================================================================

enum class Opcode : uint8_t {
    // Control
    NOP      = 0x00,
    RET      = 0x01,  // Return r0

    // Load/Move
    LOAD_C   = 0x02,  // dst = constants[imm12]
    LOAD_X   = 0x03,  // dst = x (domain value)
    LOAD_T   = 0x04,  // dst = t (time)
    LOAD_S   = 0x05,  // dst = sampler[imm12 & 0x7]
    MOV      = 0x06,  // dst = src1

    // Arithmetic
    ADD      = 0x10,  // dst = src1 + src2
    SUB      = 0x11,  // dst = src1 - src2
    MUL      = 0x12,  // dst = src1 * src2
    DIV      = 0x13,  // dst = src1 / src2
    NEG      = 0x14,  // dst = -src1
    MOD      = 0x15,  // dst = mod(src1, src2)

    // Transcendentals
    SIN      = 0x20,  // dst = sin(src1)
    COS      = 0x21,  // dst = cos(src1)
    TAN      = 0x22,  // dst = tan(src1)
    ASIN     = 0x23,  // dst = asin(src1)
    ACOS     = 0x24,  // dst = acos(src1)
    ATAN     = 0x25,  // dst = atan(src1)
    ATAN2    = 0x26,  // dst = atan2(src1, src2)
    SINH     = 0x27,  // dst = sinh(src1)
    COSH     = 0x28,  // dst = cosh(src1)
    TANH     = 0x29,  // dst = tanh(src1)

    // Exponential/Logarithmic
    EXP      = 0x30,  // dst = exp(src1)
    EXP2     = 0x31,  // dst = exp2(src1)
    LOG      = 0x32,  // dst = log(src1) (natural)
    LOG2     = 0x33,  // dst = log2(src1)
    POW      = 0x34,  // dst = pow(src1, src2)
    SQRT     = 0x35,  // dst = sqrt(src1)
    RSQRT    = 0x36,  // dst = inverseSqrt(src1)

    // Utility
    ABS      = 0x40,  // dst = abs(src1)
    MIN      = 0x41,  // dst = min(src1, src2)
    MAX      = 0x42,  // dst = max(src1, src2)
    FLOOR    = 0x43,  // dst = floor(src1)
    CEIL     = 0x44,  // dst = ceil(src1)
    ROUND    = 0x45,  // dst = round(src1)
    FRACT    = 0x46,  // dst = fract(src1)
    SIGN     = 0x47,  // dst = sign(src1)
    CLAMP01  = 0x48,  // dst = clamp(src1, 0, 1)
    STEP     = 0x49,  // dst = step(src1, src2) - src1 is edge
    MIX      = 0x4A,  // dst = mix(src1, src2, r[imm12 & 0xF])

    // Extended (two-word instructions if needed later)
    // Reserved 0xF0-0xFF
};

//=============================================================================
// Instruction encoding/decoding
//=============================================================================

struct Instruction {
    uint32_t encoded;

    Instruction() : encoded(0) {}
    Instruction(uint32_t e) : encoded(e) {}

    // Encode instruction
    static Instruction encode(Opcode op, uint8_t dst, uint8_t src1 = 0,
                              uint8_t src2 = 0, uint16_t imm12 = 0) {
        return Instruction(
            (static_cast<uint32_t>(op) << 24) |
            ((dst & 0xF) << 20) |
            ((src1 & 0xF) << 16) |
            ((src2 & 0xF) << 12) |
            (imm12 & 0xFFF)
        );
    }

    // Decode instruction
    Opcode opcode() const { return static_cast<Opcode>((encoded >> 24) & 0xFF); }
    uint8_t dst() const { return (encoded >> 20) & 0xF; }
    uint8_t src1() const { return (encoded >> 16) & 0xF; }
    uint8_t src2() const { return (encoded >> 12) & 0xF; }
    uint16_t imm12() const { return encoded & 0xFFF; }
};

//=============================================================================
// Input types (for LOAD_S)
//=============================================================================

enum class InputSlot : uint8_t {
    X = 0,       // Domain x value
    T = 1,       // Time
    S0 = 2,      // Sampler 0
    S1 = 3,      // Sampler 1
    S2 = 4,      // Sampler 2
    S3 = 5,      // Sampler 3
    S4 = 6,      // Sampler 4
    S5 = 7,      // Sampler 5
    S6 = 8,      // Sampler 6
    S7 = 9,      // Sampler 7
};

//=============================================================================
// Program - compiled bytecode for one or more functions
//=============================================================================

struct Function {
    uint32_t codeOffset;      // Offset into code array
    uint32_t codeLength;      // Number of instructions
};

struct Program {
    std::vector<float> constants;       // Constant pool
    std::vector<uint32_t> code;         // Bytecode (Instruction::encoded)
    std::vector<Function> functions;    // Function table

    // Metadata about the compiled expression
    bool usesTime = false;              // Uses 't' or 'time' variable
    bool usesX = false;                 // Uses 'x' variable
    std::vector<int> bufferRefs;        // Buffer indices referenced

    // Serialize to buffer for GPU upload
    // Layout:
    //   [0] magic
    //   [1] version
    //   [2] function count
    //   [3] constant count
    //   [4..4+funcCount*2] function table (offset, length pairs)
    //   [4+funcCount*2..] constants (as u32 bitcast from f32)
    //   [...] code
    std::vector<uint32_t> serialize() const;

    // Deserialize from buffer
    static Result<Program> deserialize(std::span<const uint32_t> data);

    // Debug: disassemble to string
    std::string disassemble() const;
};

//=============================================================================
// Helper to build instructions
//=============================================================================

class InstructionBuilder {
public:
    // Load constant value, returns register containing it
    uint8_t loadConst(float value);

    // Load input
    void loadX(uint8_t dst);
    void loadT(uint8_t dst);
    void loadSampler(uint8_t dst, uint8_t samplerIdx);

    // Move
    void mov(uint8_t dst, uint8_t src);

    // Arithmetic
    void add(uint8_t dst, uint8_t src1, uint8_t src2);
    void sub(uint8_t dst, uint8_t src1, uint8_t src2);
    void mul(uint8_t dst, uint8_t src1, uint8_t src2);
    void div(uint8_t dst, uint8_t src1, uint8_t src2);
    void neg(uint8_t dst, uint8_t src);
    void mod(uint8_t dst, uint8_t src1, uint8_t src2);

    // Transcendentals
    void sin(uint8_t dst, uint8_t src);
    void cos(uint8_t dst, uint8_t src);
    void tan(uint8_t dst, uint8_t src);
    void asin(uint8_t dst, uint8_t src);
    void acos(uint8_t dst, uint8_t src);
    void atan(uint8_t dst, uint8_t src);
    void atan2(uint8_t dst, uint8_t y, uint8_t x);
    void sinh(uint8_t dst, uint8_t src);
    void cosh(uint8_t dst, uint8_t src);
    void tanh(uint8_t dst, uint8_t src);

    // Exp/Log
    void exp(uint8_t dst, uint8_t src);
    void exp2(uint8_t dst, uint8_t src);
    void log(uint8_t dst, uint8_t src);
    void log2(uint8_t dst, uint8_t src);
    void pow(uint8_t dst, uint8_t base, uint8_t exponent);
    void sqrt(uint8_t dst, uint8_t src);
    void rsqrt(uint8_t dst, uint8_t src);

    // Utility
    void abs(uint8_t dst, uint8_t src);
    void min(uint8_t dst, uint8_t src1, uint8_t src2);
    void max(uint8_t dst, uint8_t src1, uint8_t src2);
    void floor(uint8_t dst, uint8_t src);
    void ceil(uint8_t dst, uint8_t src);
    void round(uint8_t dst, uint8_t src);
    void fract(uint8_t dst, uint8_t src);
    void sign(uint8_t dst, uint8_t src);
    void clamp01(uint8_t dst, uint8_t src);
    void step(uint8_t dst, uint8_t edge, uint8_t x);
    void mix(uint8_t dst, uint8_t a, uint8_t b, uint8_t t);

    // Return (implicitly returns r0)
    void ret();

    // Build program (moves internal state, builder is empty after this)
    Program build();

    // Allocate a temporary register
    uint8_t allocReg();
    void freeReg(uint8_t reg);

    // Start a new function, returns function index
    uint32_t beginFunction();
    void endFunction();

    // Metadata tracking
    void markUsesTime() { _usesTime = true; }
    void markUsesX() { _usesX = true; }
    void addBufferRef(int idx);

private:
    std::vector<float> _constants;
    std::vector<uint32_t> _code;
    std::vector<Function> _functions;
    std::vector<int> _bufferRefs;
    uint32_t _currentFuncStart = 0;
    uint16_t _regAlloc = 0;  // Bitmask of allocated registers
    bool _usesTime = false;
    bool _usesX = false;

    void emit(Instruction inst);
    uint16_t addConstant(float value);
};

//=============================================================================
// Utility
//=============================================================================

// Get opcode name for debugging
const char* opcodeName(Opcode op);

} // namespace yetty::yfsvm
