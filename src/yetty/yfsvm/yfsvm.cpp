#include "yfsvm.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace yetty::yfsvm {

//=============================================================================
// Program serialization
//=============================================================================

std::vector<uint32_t> Program::serialize() const {
    std::vector<uint32_t> buffer;

    // Header
    buffer.push_back(YFSVM_MAGIC);
    buffer.push_back(YFSVM_VERSION);
    buffer.push_back(static_cast<uint32_t>(functions.size()));
    buffer.push_back(static_cast<uint32_t>(constants.size()));

    // Function table (offset, length pairs as packed u32)
    for (const auto& func : functions) {
        // Pack: lower 16 bits = offset, upper 16 bits = length
        buffer.push_back((func.codeLength << 16) | (func.codeOffset & 0xFFFF));
    }
    // Pad to YFSVM_MAX_FUNCTIONS entries
    for (size_t i = functions.size(); i < YFSVM_MAX_FUNCTIONS; ++i) {
        buffer.push_back(0);
    }

    // Constants (bitcast f32 to u32)
    for (float c : constants) {
        uint32_t bits;
        std::memcpy(&bits, &c, sizeof(bits));
        buffer.push_back(bits);
    }

    // Code
    for (uint32_t instr : code) {
        buffer.push_back(instr);
    }

    return buffer;
}

Result<Program> Program::deserialize(std::span<const uint32_t> data) {
    if (data.size() < 4 + YFSVM_MAX_FUNCTIONS) {
        return Err<Program>("buffer too small for yfsvm header");
    }

    if (data[0] != YFSVM_MAGIC) {
        return Err<Program>("invalid yfsvm magic number");
    }

    if (data[1] != YFSVM_VERSION) {
        return Err<Program>("unsupported yfsvm version");
    }

    Program prog;
    uint32_t funcCount = data[2];
    uint32_t constCount = data[3];

    if (funcCount > YFSVM_MAX_FUNCTIONS) {
        return Err<Program>("function count exceeds maximum");
    }

    // Read function table
    for (uint32_t i = 0; i < funcCount; ++i) {
        uint32_t packed = data[4 + i];
        Function func;
        func.codeOffset = packed & 0xFFFF;
        func.codeLength = packed >> 16;
        prog.functions.push_back(func);
    }

    size_t constOffset = 4 + YFSVM_MAX_FUNCTIONS;
    if (data.size() < constOffset + constCount) {
        return Err<Program>("buffer too small for constants");
    }

    // Read constants
    for (uint32_t i = 0; i < constCount; ++i) {
        uint32_t bits = data[constOffset + i];
        float value;
        std::memcpy(&value, &bits, sizeof(value));
        prog.constants.push_back(value);
    }

    // Read code
    size_t codeOffset = constOffset + constCount;
    while (codeOffset < data.size()) {
        prog.code.push_back(data[codeOffset++]);
    }

    return Ok(std::move(prog));
}

std::string Program::disassemble() const {
    std::ostringstream out;

    out << "; yfsvm program\n";
    out << "; functions: " << functions.size() << "\n";
    out << "; constants: " << constants.size() << "\n";
    out << "; code size: " << code.size() << " instructions\n\n";

    // Print constants
    out << "; Constants:\n";
    for (size_t i = 0; i < constants.size(); ++i) {
        out << ";   c" << i << " = " << constants[i] << "\n";
    }
    out << "\n";

    // Print functions
    for (size_t fi = 0; fi < functions.size(); ++fi) {
        const auto& func = functions[fi];
        out << "function_" << fi << ":  ; offset=" << func.codeOffset
            << ", length=" << func.codeLength << "\n";

        for (uint32_t i = 0; i < func.codeLength; ++i) {
            uint32_t pc = func.codeOffset + i;
            if (pc >= code.size()) break;

            Instruction instr(code[pc]);
            out << "  " << std::setw(4) << i << ": ";
            out << opcodeName(instr.opcode());

            switch (instr.opcode()) {
                case Opcode::NOP:
                case Opcode::RET:
                    break;

                case Opcode::LOAD_C:
                    out << " r" << (int)instr.dst() << ", c" << instr.imm12();
                    if (instr.imm12() < constants.size()) {
                        out << "  ; " << constants[instr.imm12()];
                    }
                    break;

                case Opcode::LOAD_X:
                case Opcode::LOAD_T:
                    out << " r" << (int)instr.dst();
                    break;

                case Opcode::LOAD_S:
                    out << " r" << (int)instr.dst() << ", s" << (instr.imm12() & 0x7);
                    break;

                case Opcode::MOV:
                case Opcode::NEG:
                case Opcode::SIN:
                case Opcode::COS:
                case Opcode::TAN:
                case Opcode::ASIN:
                case Opcode::ACOS:
                case Opcode::ATAN:
                case Opcode::SINH:
                case Opcode::COSH:
                case Opcode::TANH:
                case Opcode::EXP:
                case Opcode::EXP2:
                case Opcode::LOG:
                case Opcode::LOG2:
                case Opcode::SQRT:
                case Opcode::RSQRT:
                case Opcode::ABS:
                case Opcode::FLOOR:
                case Opcode::CEIL:
                case Opcode::ROUND:
                case Opcode::FRACT:
                case Opcode::SIGN:
                case Opcode::CLAMP01:
                    out << " r" << (int)instr.dst() << ", r" << (int)instr.src1();
                    break;

                case Opcode::ADD:
                case Opcode::SUB:
                case Opcode::MUL:
                case Opcode::DIV:
                case Opcode::MOD:
                case Opcode::ATAN2:
                case Opcode::POW:
                case Opcode::MIN:
                case Opcode::MAX:
                case Opcode::STEP:
                    out << " r" << (int)instr.dst() << ", r" << (int)instr.src1()
                        << ", r" << (int)instr.src2();
                    break;

                case Opcode::MIX:
                    out << " r" << (int)instr.dst() << ", r" << (int)instr.src1()
                        << ", r" << (int)instr.src2() << ", r" << (instr.imm12() & 0xF);
                    break;

                default:
                    out << " ???";
                    break;
            }
            out << "\n";
        }
        out << "\n";
    }

    return out.str();
}

//=============================================================================
// InstructionBuilder
//=============================================================================

void InstructionBuilder::emit(Instruction inst) {
    _code.push_back(inst.encoded);
}

uint16_t InstructionBuilder::addConstant(float value) {
    // Check if constant already exists
    for (size_t i = 0; i < _constants.size(); ++i) {
        // Use bitwise comparison for exact match (handles -0.0 vs +0.0)
        uint32_t a, b;
        std::memcpy(&a, &_constants[i], sizeof(a));
        std::memcpy(&b, &value, sizeof(b));
        if (a == b) {
            return static_cast<uint16_t>(i);
        }
    }
    // Add new constant
    uint16_t idx = static_cast<uint16_t>(_constants.size());
    _constants.push_back(value);
    return idx;
}

uint8_t InstructionBuilder::loadConst(float value) {
    uint8_t reg = allocReg();
    uint16_t idx = addConstant(value);
    emit(Instruction::encode(Opcode::LOAD_C, reg, 0, 0, idx));
    return reg;
}

void InstructionBuilder::loadX(uint8_t dst) {
    emit(Instruction::encode(Opcode::LOAD_X, dst));
}

void InstructionBuilder::loadT(uint8_t dst) {
    emit(Instruction::encode(Opcode::LOAD_T, dst));
}

void InstructionBuilder::loadSampler(uint8_t dst, uint8_t samplerIdx) {
    emit(Instruction::encode(Opcode::LOAD_S, dst, 0, 0, samplerIdx & 0x7));
}

void InstructionBuilder::mov(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::MOV, dst, src));
}

void InstructionBuilder::add(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::ADD, dst, src1, src2));
}

void InstructionBuilder::sub(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::SUB, dst, src1, src2));
}

void InstructionBuilder::mul(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::MUL, dst, src1, src2));
}

void InstructionBuilder::div(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::DIV, dst, src1, src2));
}

void InstructionBuilder::neg(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::NEG, dst, src));
}

void InstructionBuilder::mod(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::MOD, dst, src1, src2));
}

void InstructionBuilder::sin(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::SIN, dst, src));
}

void InstructionBuilder::cos(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::COS, dst, src));
}

void InstructionBuilder::tan(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::TAN, dst, src));
}

void InstructionBuilder::asin(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::ASIN, dst, src));
}

void InstructionBuilder::acos(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::ACOS, dst, src));
}

void InstructionBuilder::atan(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::ATAN, dst, src));
}

void InstructionBuilder::atan2(uint8_t dst, uint8_t y, uint8_t x) {
    emit(Instruction::encode(Opcode::ATAN2, dst, y, x));
}

void InstructionBuilder::sinh(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::SINH, dst, src));
}

void InstructionBuilder::cosh(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::COSH, dst, src));
}

void InstructionBuilder::tanh(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::TANH, dst, src));
}

void InstructionBuilder::exp(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::EXP, dst, src));
}

void InstructionBuilder::exp2(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::EXP2, dst, src));
}

void InstructionBuilder::log(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::LOG, dst, src));
}

void InstructionBuilder::log2(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::LOG2, dst, src));
}

void InstructionBuilder::pow(uint8_t dst, uint8_t base, uint8_t exponent) {
    emit(Instruction::encode(Opcode::POW, dst, base, exponent));
}

void InstructionBuilder::sqrt(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::SQRT, dst, src));
}

void InstructionBuilder::rsqrt(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::RSQRT, dst, src));
}

void InstructionBuilder::abs(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::ABS, dst, src));
}

void InstructionBuilder::min(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::MIN, dst, src1, src2));
}

void InstructionBuilder::max(uint8_t dst, uint8_t src1, uint8_t src2) {
    emit(Instruction::encode(Opcode::MAX, dst, src1, src2));
}

void InstructionBuilder::floor(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::FLOOR, dst, src));
}

void InstructionBuilder::ceil(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::CEIL, dst, src));
}

void InstructionBuilder::round(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::ROUND, dst, src));
}

void InstructionBuilder::fract(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::FRACT, dst, src));
}

void InstructionBuilder::sign(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::SIGN, dst, src));
}

void InstructionBuilder::clamp01(uint8_t dst, uint8_t src) {
    emit(Instruction::encode(Opcode::CLAMP01, dst, src));
}

void InstructionBuilder::step(uint8_t dst, uint8_t edge, uint8_t x) {
    emit(Instruction::encode(Opcode::STEP, dst, edge, x));
}

void InstructionBuilder::mix(uint8_t dst, uint8_t a, uint8_t b, uint8_t t) {
    emit(Instruction::encode(Opcode::MIX, dst, a, b, t & 0xF));
}

void InstructionBuilder::ret() {
    emit(Instruction::encode(Opcode::RET, 0));
}

uint8_t InstructionBuilder::allocReg() {
    for (uint8_t i = 0; i < YFSVM_MAX_REGISTERS; ++i) {
        if ((_regAlloc & (1 << i)) == 0) {
            _regAlloc |= (1 << i);
            return i;
        }
    }
    // Out of registers, return r15 (will overwrite)
    return 15;
}

void InstructionBuilder::freeReg(uint8_t reg) {
    _regAlloc &= ~(1 << reg);
}

uint32_t InstructionBuilder::beginFunction() {
    _currentFuncStart = static_cast<uint32_t>(_code.size());
    _regAlloc = 0;  // Reset register allocation for new function
    return static_cast<uint32_t>(_functions.size());
}

void InstructionBuilder::endFunction() {
    Function func;
    func.codeOffset = _currentFuncStart;
    func.codeLength = static_cast<uint32_t>(_code.size()) - _currentFuncStart;
    _functions.push_back(func);
}

void InstructionBuilder::addBufferRef(int idx) {
    for (int ref : _bufferRefs) {
        if (ref == idx) return;  // Already tracked
    }
    _bufferRefs.push_back(idx);
}

Program InstructionBuilder::build() {
    Program prog;
    prog.constants = std::move(_constants);
    prog.code = std::move(_code);
    prog.functions = std::move(_functions);
    prog.usesTime = _usesTime;
    prog.usesX = _usesX;
    prog.bufferRefs = std::move(_bufferRefs);

    // Reset builder state
    _constants.clear();
    _code.clear();
    _functions.clear();
    _bufferRefs.clear();
    _currentFuncStart = 0;
    _regAlloc = 0;
    _usesTime = false;
    _usesX = false;

    return prog;
}

//=============================================================================
// Utility
//=============================================================================

const char* opcodeName(Opcode op) {
    switch (op) {
        case Opcode::NOP:      return "NOP";
        case Opcode::RET:      return "RET";
        case Opcode::LOAD_C:   return "LOAD_C";
        case Opcode::LOAD_X:   return "LOAD_X";
        case Opcode::LOAD_T:   return "LOAD_T";
        case Opcode::LOAD_S:   return "LOAD_S";
        case Opcode::MOV:      return "MOV";
        case Opcode::ADD:      return "ADD";
        case Opcode::SUB:      return "SUB";
        case Opcode::MUL:      return "MUL";
        case Opcode::DIV:      return "DIV";
        case Opcode::NEG:      return "NEG";
        case Opcode::MOD:      return "MOD";
        case Opcode::SIN:      return "SIN";
        case Opcode::COS:      return "COS";
        case Opcode::TAN:      return "TAN";
        case Opcode::ASIN:     return "ASIN";
        case Opcode::ACOS:     return "ACOS";
        case Opcode::ATAN:     return "ATAN";
        case Opcode::ATAN2:    return "ATAN2";
        case Opcode::SINH:     return "SINH";
        case Opcode::COSH:     return "COSH";
        case Opcode::TANH:     return "TANH";
        case Opcode::EXP:      return "EXP";
        case Opcode::EXP2:     return "EXP2";
        case Opcode::LOG:      return "LOG";
        case Opcode::LOG2:     return "LOG2";
        case Opcode::POW:      return "POW";
        case Opcode::SQRT:     return "SQRT";
        case Opcode::RSQRT:    return "RSQRT";
        case Opcode::ABS:      return "ABS";
        case Opcode::MIN:      return "MIN";
        case Opcode::MAX:      return "MAX";
        case Opcode::FLOOR:    return "FLOOR";
        case Opcode::CEIL:     return "CEIL";
        case Opcode::ROUND:    return "ROUND";
        case Opcode::FRACT:    return "FRACT";
        case Opcode::SIGN:     return "SIGN";
        case Opcode::CLAMP01:  return "CLAMP01";
        case Opcode::STEP:     return "STEP";
        case Opcode::MIX:      return "MIX";
        default:               return "???";
    }
}

} // namespace yetty::yfsvm
