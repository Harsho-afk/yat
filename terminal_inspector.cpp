#include <iostream>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (isatty(STDIN_FILENO)) {
        std::cout << "Input is a terminal: Yes\n";
        if(const char *name = ttyname(STDIN_FILENO)) {
            std::cout << name << "\n";
        }
    } else {
        std::cout << "Input is a terminal: No\n";
    }
    if (isatty(STDOUT_FILENO)) {
        std::cout << "Output is a terminal: Yes\n";
        if(const char *name = ttyname(STDOUT_FILENO)) {
            std::cout << name << "\n";
        }
    } else {
        std::cout << "Output is a terminal: No\n";
    }
    int terminal_fd = STDOUT_FILENO;

    if (!isatty(terminal_fd))
        terminal_fd = STDIN_FILENO;
    winsize win_size{};
    if (ioctl(terminal_fd, TIOCGWINSZ, &win_size) == 0) {
        std::cout << "Rows: " << win_size.ws_row << "\n";
        std::cout << "Cols: " << win_size.ws_col << "\n";
    }
    termios terminal_attr{};
    if (tcgetattr(STDIN_FILENO, &terminal_attr) == 0) {
        std::cout << "tcgetattr works\n";
    }
    return 0;
}
