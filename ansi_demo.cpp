#include <cstdio>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

enum class Color : int {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Magenta = 5,
    Cyan = 6,
    White = 7
};

void clear_screen() { printf("\x1b[2J\x1b[H"); }

int move_cursor(int x, int y) {
    winsize win_size{};
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &win_size) == -1)
        return 0;
    if (x >= win_size.ws_col || y >= win_size.ws_row || x < 0 || y < 0)
        return 0;
    printf("\x1b[%d;%dH", y + 1, x + 1);
    return 1;
}

int set_color(int colorid) {
    if (colorid < 0 || colorid > 255)
        return 0;

    printf("\x1b[38;5;%dm", colorid);
    return 1;
}

int set_color(Color colorid) { return set_color(static_cast<int>(colorid)); }

void reset_style() { printf("\x1b[0m"); }

void hide_cursor() { printf("\x1b[?25l"); }

void show_cursor() { printf("\x1b[?25h"); }

int main(int argc, char *argv[]) {
    if (!isatty(STDOUT_FILENO))
        return 0;
    clear_screen();
    move_cursor(10, 5);
    set_color(Color::Green);
    printf("Test\n");
    reset_style();
    printf("Test\n");
    hide_cursor();
    show_cursor();
    return 0;
}
