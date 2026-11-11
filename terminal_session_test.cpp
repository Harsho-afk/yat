#include "terminal_session.hpp"
#include <iostream>
#include <termios.h>
#include <utility>

bool same_terminal_settings(const termios &a, const termios &b) {
    return a.c_iflag == b.c_iflag && a.c_oflag == b.c_oflag &&
           a.c_cflag == b.c_cflag && a.c_lflag == b.c_lflag &&
           a.c_cc[VMIN] == b.c_cc[VMIN] && a.c_cc[VTIME] == b.c_cc[VTIME];
}

int main() {
    termios before{};

    if (tcgetattr(STDIN_FILENO, &before) == -1) {
        std::cerr << "Cannot read initial terminal settings.\n";
        return 1;
    }

    std::cout << "[1] Initial terminal settings captured.\n";

    {
        TerminalSession first;

        if (!first.initialize()) {
            std::cerr
                << "Initialization failed. Is this running in a terminal?\n";
            return 1;
        }

        std::cout << "[2] Initialization: "
                  << (first.is_active() ? "PASS" : "FAIL") << '\n';

        std::cout << "[3] Double initialization: "
                  << (!first.initialize() ? "PASS" : "FAIL") << '\n';

        {
            TerminalSession second(std::move(first));

            std::cout << "[4] Move construction: "
                      << (!first.is_active() && second.is_active() ? "PASS"
                                                                   : "FAIL")
                      << '\n';
        }

        std::cout << "[5] Moved-to object destruction completed.\n";
    }

    termios after{};

    if (tcgetattr(STDIN_FILENO, &after) == -1) {
        std::cerr << "Cannot read terminal settings after cleanup.\n";
        return 1;
    }

    std::cout << "[6] Terminal restoration: "
              << (same_terminal_settings(before, after) ? "PASS" : "FAIL")
              << '\n';

    {
        TerminalSession session;

        std::cout << "[7] Restore without initialization: "
                  << (session.restore() ? "PASS" : "FAIL") << '\n';

        std::cout << "[8] Repeated restore: "
                  << (session.restore() ? "PASS" : "FAIL") << '\n';
    }

    std::cout << "\nTests completed.\n";
    return 0;
}
