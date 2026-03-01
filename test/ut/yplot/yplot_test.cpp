//=============================================================================
// YPlot Tests
//
// Tests for the YPlot high-level plotting library
//=============================================================================

// Include C++ standard headers before boost/ut.hpp
#include <cstddef>
#include <version>
#include <algorithm>
#include <cmath>
#include <array>

#include <boost/ut.hpp>
#include "yetty/yplot/yplot.h"
#include "yetty/yfsvm/yfsvm.h"
#include "yetty/yfsvm/yfsvm-compiler.h"
#include "yetty/ydraw/ydraw-buffer.h"

using namespace boost::ut;
using namespace yetty;
using namespace yetty::yplot;

//=============================================================================
// CPU Evaluator (same as in yfsvm tests)
//=============================================================================

namespace {

struct CPUEvaluator {
    std::array<float, 16> regs{};
    const float* constants = nullptr;
    const uint32_t* code = nullptr;
    float x = 0.0f;
    float t = 0.0f;
    std::array<float, 8> samplers{};

    float evaluate(const yfsvm::Program& prog, uint32_t funcIdx) {
        using namespace yfsvm;

        if (funcIdx >= prog.functions.size()) return 0.0f;

        constants = prog.constants.data();
        code = prog.code.data();

        const auto& func = prog.functions[funcIdx];
        uint32_t pc = func.codeOffset;
        uint32_t endPc = pc + func.codeLength;

        regs.fill(0.0f);

        while (pc < endPc) {
            yfsvm::Instruction instr(code[pc++]);
            auto op = instr.opcode();
            uint8_t dst = instr.dst();
            uint8_t src1 = instr.src1();
            uint8_t src2 = instr.src2();
            uint16_t imm = instr.imm12();

            switch (op) {
            case Opcode::NOP: break;
            case Opcode::RET: return regs[0];
            case Opcode::LOAD_C: regs[dst] = constants[imm]; break;
            case Opcode::LOAD_X: regs[dst] = x; break;
            case Opcode::LOAD_T: regs[dst] = t; break;
            case Opcode::LOAD_S: regs[dst] = samplers[imm & 7]; break;
            case Opcode::MOV: regs[dst] = regs[src1]; break;

            case Opcode::ADD: regs[dst] = regs[src1] + regs[src2]; break;
            case Opcode::SUB: regs[dst] = regs[src1] - regs[src2]; break;
            case Opcode::MUL: regs[dst] = regs[src1] * regs[src2]; break;
            case Opcode::DIV: regs[dst] = regs[src1] / regs[src2]; break;
            case Opcode::NEG: regs[dst] = -regs[src1]; break;
            case Opcode::MOD: regs[dst] = std::fmod(regs[src1], regs[src2]); break;

            case Opcode::SIN: regs[dst] = std::sin(regs[src1]); break;
            case Opcode::COS: regs[dst] = std::cos(regs[src1]); break;
            case Opcode::TAN: regs[dst] = std::tan(regs[src1]); break;
            case Opcode::ASIN: regs[dst] = std::asin(regs[src1]); break;
            case Opcode::ACOS: regs[dst] = std::acos(regs[src1]); break;
            case Opcode::ATAN: regs[dst] = std::atan(regs[src1]); break;
            case Opcode::ATAN2: regs[dst] = std::atan2(regs[src1], regs[src2]); break;
            case Opcode::SINH: regs[dst] = std::sinh(regs[src1]); break;
            case Opcode::COSH: regs[dst] = std::cosh(regs[src1]); break;
            case Opcode::TANH: regs[dst] = std::tanh(regs[src1]); break;

            case Opcode::EXP: regs[dst] = std::exp(regs[src1]); break;
            case Opcode::EXP2: regs[dst] = std::exp2(regs[src1]); break;
            case Opcode::LOG: regs[dst] = std::log(regs[src1]); break;
            case Opcode::LOG2: regs[dst] = std::log2(regs[src1]); break;
            case Opcode::POW: regs[dst] = std::pow(regs[src1], regs[src2]); break;
            case Opcode::SQRT: regs[dst] = std::sqrt(regs[src1]); break;
            case Opcode::RSQRT: regs[dst] = 1.0f / std::sqrt(regs[src1]); break;

            case Opcode::ABS: regs[dst] = std::abs(regs[src1]); break;
            case Opcode::MIN: regs[dst] = std::min(regs[src1], regs[src2]); break;
            case Opcode::MAX: regs[dst] = std::max(regs[src1], regs[src2]); break;
            case Opcode::FLOOR: regs[dst] = std::floor(regs[src1]); break;
            case Opcode::CEIL: regs[dst] = std::ceil(regs[src1]); break;
            case Opcode::ROUND: regs[dst] = std::round(regs[src1]); break;
            case Opcode::FRACT: regs[dst] = regs[src1] - std::floor(regs[src1]); break;
            case Opcode::SIGN: regs[dst] = (regs[src1] > 0.0f) ? 1.0f : ((regs[src1] < 0.0f) ? -1.0f : 0.0f); break;
            case Opcode::CLAMP01: regs[dst] = std::clamp(regs[src1], 0.0f, 1.0f); break;
            case Opcode::STEP: regs[dst] = (regs[src2] < regs[src1]) ? 0.0f : 1.0f; break;
            case Opcode::MIX: {
                float tVal = regs[imm & 0xF];
                regs[dst] = regs[src1] * (1.0f - tVal) + regs[src2] * tVal;
                break;
            }
            default: break;
            }
        }
        return regs[0];
    }
};

} // anonymous namespace

//=============================================================================
// YPlotState creation tests
//=============================================================================

suite yplotstate_creation_tests = [] {
    "YPlotState::create returns valid pointer"_test = [] {
        auto result = YPlotState::create();
        expect(result.has_value());
        expect(*result != nullptr);
    };

    "new YPlotState has zero functions"_test = [] {
        auto result = YPlotState::create();
        expect(result.has_value());
        expect((*result)->functionCount() == 0_u);
    };

    "new YPlotState has default range"_test = [] {
        auto result = YPlotState::create();
        expect(result.has_value());
        const auto& range = (*result)->range();
        expect(range.xMin < range.xMax);
        expect(range.yMin < range.yMax);
    };
};

//=============================================================================
// YPlotState function management tests
//=============================================================================

suite yplotstate_function_tests = [] {
    "addFunction increases count"_test = [] {
        auto state = *YPlotState::create();
        expect(state->functionCount() == 0_u);

        auto res = state->addFunction("f", "sin(x)");
        expect(res.has_value());
        expect(state->functionCount() == 1_u);
    };

    "addFunction with color"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->addFunction("f", "sin(x)", 0xFFFF0000);
        expect(res.has_value());
        expect(state->functions()[0].color == 0xFFFF0000u);
    };

    "addFunction auto-assigns color"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        expect(state->functions()[0].color != 0u);
    };

    "multiple addFunction calls"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        state->addFunction("g", "cos(x)");
        state->addFunction("h", "tan(x)");
        expect(state->functionCount() == 3_u);
    };

    "clearFunctions resets count"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        state->addFunction("g", "cos(x)");
        state->clearFunctions();
        expect(state->functionCount() == 0_u);
    };

    "removeFunction decreases count"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        state->addFunction("g", "cos(x)");
        state->removeFunction(0);
        expect(state->functionCount() == 1_u);
    };

    "setFunction updates expression"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        auto res = state->setFunction(0, "cos(x)");
        expect(res.has_value());
    };

    "setFunctionColor updates color"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("f", "sin(x)");
        state->setFunctionColor(0, 0xFF00FF00);
        expect(state->functions()[0].color == 0xFF00FF00u);
    };
};

//=============================================================================
// Expression parsing tests
//=============================================================================

suite expression_parsing_tests = [] {
    "parseExpressions single function"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->parseExpressions("f = sin(x)");
        expect(res.has_value());
        expect(state->functionCount() == 1_u);
    };

    "parseExpressions multiple functions"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->parseExpressions("f = sin(x); g = cos(x)");
        expect(res.has_value());
        expect(state->functionCount() == 2_u);
    };

    "parseExpressions three functions"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->parseExpressions("f = sin(x); g = cos(x); h = tan(x)");
        expect(res.has_value());
        expect(state->functionCount() == 3_u);
    };

    "parseExpressions clears previous"_test = [] {
        auto state = *YPlotState::create();
        state->addFunction("old", "x");
        state->parseExpressions("f = sin(x)");
        expect(state->functionCount() == 1_u);
    };

    "parseExpressions assigns names"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("wave = sin(x)");
        expect(state->functions()[0].name == std::string("wave"));
    };
};

//=============================================================================
// Compilation tests
//=============================================================================

suite compilation_tests = [] {
    "compile returns program"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x)");
        auto prog = state->compile();
        expect(prog.has_value());
    };

    "compile creates one function per expression"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x); g = cos(x)");
        auto prog = state->compile();
        expect(prog.has_value());
        expect(prog->functions.size() == 2_u);
    };

    "compiled program is executable"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x)");
        auto prog = state->compile();
        expect(prog.has_value());

        CPUEvaluator eval;
        eval.x = 0.0f;
        float result = eval.evaluate(*prog, 0);
        expect(std::abs(result - 0.0f) < 0.001f);  // sin(0) = 0
    };

    "compiled sin evaluates correctly"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x)");
        auto prog = state->compile();

        CPUEvaluator eval;
        eval.x = 3.14159265f / 2.0f;
        float result = eval.evaluate(*prog, 0);
        expect(std::abs(result - 1.0f) < 0.001f);  // sin(pi/2) = 1
    };

    "compiled cos evaluates correctly"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = cos(x)");
        auto prog = state->compile();

        CPUEvaluator eval;
        eval.x = 0.0f;
        float result = eval.evaluate(*prog, 0);
        expect(std::abs(result - 1.0f) < 0.001f);  // cos(0) = 1
    };

    "compiled multi-function evaluates all correctly"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x); g = cos(x); h = x*x");
        auto prog = state->compile();
        expect(prog.has_value());

        CPUEvaluator eval;
        eval.x = 0.0f;

        float f_result = eval.evaluate(*prog, 0);
        float g_result = eval.evaluate(*prog, 1);
        float h_result = eval.evaluate(*prog, 2);

        expect(std::abs(f_result - 0.0f) < 0.001f);  // sin(0) = 0
        expect(std::abs(g_result - 1.0f) < 0.001f);  // cos(0) = 1
        expect(std::abs(h_result - 0.0f) < 0.001f);  // 0*0 = 0
    };

    "compiled complex expression evaluates correctly"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x) * cos(x)");
        auto prog = state->compile();

        CPUEvaluator eval;
        eval.x = 1.0f;
        float result = eval.evaluate(*prog, 0);
        float expected = std::sin(1.0f) * std::cos(1.0f);
        expect(std::abs(result - expected) < 0.001f);
    };
};

//=============================================================================
// Serialization tests
//=============================================================================

suite yplot_serialization_tests = [] {
    "compiled program serializes"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x)");
        auto prog = state->compile();
        expect(prog.has_value());

        auto buffer = prog->serialize();
        expect(!buffer.empty());
    };

    "serialized program deserializes"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x); g = cos(x)");
        auto prog = state->compile();

        auto buffer = prog->serialize();
        auto restored = yfsvm::Program::deserialize(buffer);
        expect(restored.has_value());
        expect(restored->functions.size() == 2_u);
    };

    "roundtrip preserves evaluation"_test = [] {
        auto state = *YPlotState::create();
        state->parseExpressions("f = sin(x) + cos(x)");
        auto prog = state->compile();

        auto buffer = prog->serialize();
        auto restored = yfsvm::Program::deserialize(buffer);

        CPUEvaluator eval1, eval2;
        eval1.x = 1.5f;
        eval2.x = 1.5f;

        float orig = eval1.evaluate(*prog, 0);
        float rest = eval2.evaluate(*restored, 0);
        expect(std::abs(orig - rest) < 0.0001f);
    };
};

//=============================================================================
// Range configuration tests
//=============================================================================

suite range_config_tests = [] {
    "setRange updates range"_test = [] {
        auto state = *YPlotState::create();
        state->setRange(-10.0f, 10.0f, -5.0f, 5.0f);

        expect(std::abs(state->range().xMin - (-10.0f)) < 0.001f);
        expect(std::abs(state->range().xMax - 10.0f) < 0.001f);
        expect(std::abs(state->range().yMin - (-5.0f)) < 0.001f);
        expect(std::abs(state->range().yMax - 5.0f) < 0.001f);
    };

    "range accessors work"_test = [] {
        auto state = *YPlotState::create();
        state->range().xMin = -1.0f;
        state->range().xMax = 1.0f;
        expect(std::abs(state->range().xMin - (-1.0f)) < 0.001f);
        expect(std::abs(state->range().xMax - 1.0f) < 0.001f);
    };
};

//=============================================================================
// Axis configuration tests
//=============================================================================

suite axis_config_tests = [] {
    "axis defaults to showing grid"_test = [] {
        auto state = *YPlotState::create();
        expect(state->axis().showGrid == true);
    };

    "axis defaults to showing axes"_test = [] {
        auto state = *YPlotState::create();
        expect(state->axis().showAxes == true);
    };

    "axis config is mutable"_test = [] {
        auto state = *YPlotState::create();
        state->axis().showGrid = false;
        state->axis().showAxes = false;
        expect(state->axis().showGrid == false);
        expect(state->axis().showAxes == false);
    };
};

//=============================================================================
// YDrawBuffer decoration tests
//=============================================================================

suite decoration_tests = [] {
    "buildDecoration returns ok"_test = [] {
        auto state = *YPlotState::create();
        auto buffer = *YDrawBuffer::create();
        auto res = state->buildDecoration(*buffer, 80, 25);
        expect(res.has_value());
    };

    "buildDecoration adds primitives"_test = [] {
        auto state = *YPlotState::create();
        state->axis().showGrid = true;
        state->axis().showAxes = true;
        auto buffer = *YDrawBuffer::create();
        state->buildDecoration(*buffer, 80, 25);
        expect(buffer->primitiveCount() > 0_u);
    };

    "buildDecoration with grid off adds fewer primitives"_test = [] {
        auto stateWithGrid = *YPlotState::create();
        stateWithGrid->axis().showGrid = true;
        auto bufferWithGrid = *YDrawBuffer::create();
        stateWithGrid->buildDecoration(*bufferWithGrid, 80, 25);

        auto stateNoGrid = *YPlotState::create();
        stateNoGrid->axis().showGrid = false;
        auto bufferNoGrid = *YDrawBuffer::create();
        stateNoGrid->buildDecoration(*bufferNoGrid, 80, 25);

        expect(bufferWithGrid->primitiveCount() > bufferNoGrid->primitiveCount());
    };
};

//=============================================================================
// Color utility tests
//=============================================================================

suite color_tests = [] {
    "parseColor parses 6-digit hex"_test = [] {
        auto result = parseColor("FF0000");
        expect(result.has_value());
        expect(*result == 0xFFFF0000u);  // Full alpha added
    };

    "parseColor parses with hash"_test = [] {
        auto result = parseColor("#00FF00");
        expect(result.has_value());
        expect(*result == 0xFF00FF00u);
    };

    "parseColor parses 8-digit hex"_test = [] {
        auto result = parseColor("80FF00FF");
        expect(result.has_value());
        expect(*result == 0x80FF00FFu);
    };

    "parseColor rejects empty"_test = [] {
        auto result = parseColor("");
        expect(!result.has_value());
    };

    "parseColor rejects invalid"_test = [] {
        auto result = parseColor("notacolor");
        expect(!result.has_value());
    };

    "defaultColorForIndex returns colors"_test = [] {
        for (uint32_t i = 0; i < 8; ++i) {
            uint32_t color = defaultColorForIndex(i);
            expect(color != 0u);
        }
    };

    "defaultColorForIndex wraps around"_test = [] {
        uint32_t color0 = defaultColorForIndex(0);
        uint32_t color8 = defaultColorForIndex(8);
        expect(color0 == color8);
    };
};

//=============================================================================
// Error handling tests
//=============================================================================

suite error_handling_tests = [] {
    "compile with no functions fails"_test = [] {
        auto state = *YPlotState::create();
        auto prog = state->compile();
        expect(!prog.has_value());
    };

    "setFunction out of range fails"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->setFunction(0, "sin(x)");
        expect(!res.has_value());
    };

    "setFunctionColor out of range fails"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->setFunctionColor(0, 0xFFFFFFFF);
        expect(!res.has_value());
    };

    "removeFunction out of range fails"_test = [] {
        auto state = *YPlotState::create();
        auto res = state->removeFunction(0);
        expect(!res.has_value());
    };
};
