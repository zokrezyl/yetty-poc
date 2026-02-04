//=============================================================================
// YAST Tests - Main Entry Point
//=============================================================================

// Include C++ standard headers BEFORE boost/ut.hpp to prevent cdb's 'version'
// file from shadowing C++20's <version> header
#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>

int main() {
    // Boost UT v2 automatically discovers and runs all tests
}
