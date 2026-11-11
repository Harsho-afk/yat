#include "terminal_session.hpp"

#include <cerrno>
#include <cstdio>
#include <unistd.h>

int main() {
    TerminalSession session;

    if (!session.initialize()) {
        std::perror("Terminal initialization");
        return 1;
    }

    while (true) {
        char c{};

        const ssize_t bytes_read = read(session.native_handle(), &c, 1);

        if (bytes_read == -1) {
            if (errno == EINTR)
                continue;

            std::perror("read");
            break;
        }

        if (bytes_read == 0)
            break;

        if (c == 'q')
            break;

        const ssize_t bytes_written =
            std::printf("0x%02X\r\n", static_cast<unsigned char>(c));
        std::fflush(stdout);
        if (bytes_written == -1) {
            if (errno == EINTR)
                continue;

            std::perror("write");
            break;
        }
    }

    if (!session.restore())
        std::fprintf(stderr, "Warning: terminal restoration failed\n");

    return 0;
}
