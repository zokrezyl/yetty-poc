#pragma once

#include <cstdint>

namespace yetty::telnet {

// Telnet command bytes (RFC 854)
enum Command : uint8_t {
    SE   = 240,  // End of subnegotiation
    NOP  = 241,  // No operation
    DM   = 242,  // Data mark
    BRK  = 243,  // Break
    IP   = 244,  // Interrupt process
    AO   = 245,  // Abort output
    AYT  = 246,  // Are you there
    EC   = 247,  // Erase character
    EL   = 248,  // Erase line
    GA   = 249,  // Go ahead
    SB   = 250,  // Subnegotiation begin
    WILL = 251,  // Will (agree to enable option)
    WONT = 252,  // Won't (refuse to enable option)
    DO   = 253,  // Do (request to enable option)
    DONT = 254,  // Don't (request to disable option)
    IAC  = 255,  // Interpret as command
};

// Telnet options
enum Option : uint8_t {
    OPT_BINARY         = 0,   // RFC 856 - Binary transmission
    OPT_ECHO           = 1,   // RFC 857 - Echo
    OPT_RCP            = 2,   // Reconnection
    OPT_SGA            = 3,   // RFC 858 - Suppress Go Ahead
    OPT_NAMS           = 4,   // Approx message size negotiation
    OPT_STATUS         = 5,   // RFC 859 - Status
    OPT_TM             = 6,   // RFC 860 - Timing mark
    OPT_RCTE           = 7,   // Remote controlled trans and echo
    OPT_NAOL           = 8,   // Output line width
    OPT_NAOP           = 9,   // Output page size
    OPT_NAOCRD         = 10,  // Output carriage-return disposition
    OPT_NAOHTS         = 11,  // Output horizontal tab stops
    OPT_NAOHTD         = 12,  // Output horizontal tab disposition
    OPT_NAOFFD         = 13,  // Output formfeed disposition
    OPT_NAOVTS         = 14,  // Output vertical tabstops
    OPT_NAOVTD         = 15,  // Output vertical tab disposition
    OPT_NAOLFD         = 16,  // Output linefeed disposition
    OPT_XASCII         = 17,  // Extended ASCII
    OPT_LOGOUT         = 18,  // RFC 727 - Logout
    OPT_BM             = 19,  // Byte macro
    OPT_DET            = 20,  // Data entry terminal
    OPT_SUPDUP         = 21,  // SUPDUP
    OPT_SUPDUPOUTPUT   = 22,  // SUPDUP output
    OPT_SNDLOC         = 23,  // Send location
    OPT_TTYPE          = 24,  // RFC 1091 - Terminal type
    OPT_EOR            = 25,  // RFC 885 - End of record
    OPT_TUID           = 26,  // TACACS user identification
    OPT_OUTMRK         = 27,  // Output marking
    OPT_TTYLOC         = 28,  // Terminal location number
    OPT_3270REGIME     = 29,  // Telnet 3270 regime
    OPT_X3PAD          = 30,  // X.3 PAD
    OPT_NAWS           = 31,  // RFC 1073 - Window size
    OPT_TSPEED         = 32,  // RFC 1079 - Terminal speed
    OPT_LFLOW          = 33,  // RFC 1372 - Remote flow control
    OPT_LINEMODE       = 34,  // RFC 1184 - Linemode
    OPT_XDISPLOC       = 35,  // X Display Location
    OPT_OLD_ENVIRON    = 36,  // Old environment variables (RFC 1408)
    OPT_AUTHENTICATION = 37,  // RFC 2941 - Authentication
    OPT_ENCRYPT        = 38,  // RFC 2946 - Encryption
    OPT_NEW_ENVIRON    = 39,  // RFC 1572 - New environment variables
    OPT_TN3270E        = 40,  // TN3270E
    OPT_CHARSET        = 42,  // RFC 2066 - Charset
    OPT_COMPORT        = 44,  // RFC 2217 - Com Port Control
    OPT_KERMIT         = 47,  // RFC 2840 - Kermit
    OPT_EXOPL          = 255, // RFC 861 - Extended options list
};

// Terminal type subnegotiation
enum TTYPESub : uint8_t {
    TTYPE_IS   = 0,
    TTYPE_SEND = 1,
};

// Environment variable subnegotiation (RFC 1572)
enum EnvironSub : uint8_t {
    ENVIRON_IS   = 0,
    ENVIRON_SEND = 1,
    ENVIRON_INFO = 2,
    ENVIRON_VAR  = 0,
    ENVIRON_VALUE = 1,
    ENVIRON_ESC  = 2,
    ENVIRON_USERVAR = 3,
};

} // namespace yetty::telnet
