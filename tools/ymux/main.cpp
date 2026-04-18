#include "ymux.h"

#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>

static void getTerminalSize(uint16_t& cols, uint16_t& rows) {
    struct winsize ws = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0 && ws.ws_row > 0) {
        cols = ws.ws_col;
        rows = ws.ws_row;
    } else {
        cols = 80;
        rows = 24;
    }
}

int main(int argc, char* argv[]) {
    uint16_t cols, rows;
    getTerminalSize(cols, rows);

    // Optional: override from command line
    for (int i = 1; i < argc - 1; i++) {
        if (std::string(argv[i]) == "-c" || std::string(argv[i]) == "--cols") {
            cols = static_cast<uint16_t>(std::atoi(argv[++i]));
        } else if (std::string(argv[i]) == "-r" || std::string(argv[i]) == "--rows") {
            rows = static_cast<uint16_t>(std::atoi(argv[++i]));
        }
    }

    ymux::Multiplexer mux(cols, rows);

    // Create initial pane with default shell
    if (!mux.addPane()) {
        fprintf(stderr, "ymux: failed to create initial pane\n");
        return 1;
    }

    return mux.run();
}
