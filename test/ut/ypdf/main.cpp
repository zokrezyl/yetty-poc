//=============================================================================
// YPdf Tests - Main entry point
//=============================================================================

// Include C++ standard headers before boost/ut.hpp
#include <cstddef>
#include <version>

#include <boost/ut.hpp>

int main() {
    return boost::ut::cfg<>.run();
}
