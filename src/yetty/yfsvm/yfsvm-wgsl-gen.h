#pragma once

// yfsvm WGSL Generator
//
// Generates the GPU-side yfsvm interpreter WGSL code from the same
// opcode definitions used by the C++ compiler. Single source of truth.

#include <string>

namespace yetty::yfsvm {

// Generate the complete yfsvm.wgsl library code
// This is the GPU interpreter that executes compiled bytecode
std::string generateWGSL();

} // namespace yetty::yfsvm
