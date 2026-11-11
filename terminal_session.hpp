#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <utility>

class TerminalSession {
  public:
    TerminalSession() = default;

    ~TerminalSession() { this->restore(); }

    TerminalSession(const TerminalSession &) = delete;
    TerminalSession &operator=(const TerminalSession &) = delete;

    TerminalSession(TerminalSession &&other) noexcept
        : terminal_fd_(std::exchange(other.terminal_fd_, -1)),
          original_(other.original_),
          active_(std::exchange(other.active_, false)) {}

    TerminalSession &operator=(TerminalSession &&other) = delete;

    bool initialize() {
        if (active_ == true || terminal_fd_ != -1)
            return false;
        if ((terminal_fd_ = open("/dev/tty", O_RDWR | O_CLOEXEC)) == -1)
            return false;
        if (tcgetattr(terminal_fd_, &original_) == -1) {
            close(terminal_fd_);
            terminal_fd_ = -1;
            return false;
        }
        termios raw = original_;
        cfmakeraw(&raw);
        if (tcsetattr(terminal_fd_, TCSAFLUSH, &raw) == -1) {
            close(terminal_fd_);
            terminal_fd_ = -1;
            return false;
        }
        active_ = true;
        return true;
    }

    bool restore() noexcept {
        if (terminal_fd_ == -1)
            return true;
        bool restored = true;
        if (active_ && tcsetattr(terminal_fd_, TCSANOW, &original_) == -1)
            restored = false;
        active_ = false;
        if (close(terminal_fd_) == -1)
            restored = false;
        terminal_fd_ = -1;
        return restored;
    }

    bool is_active() const noexcept { return active_; }

    int native_handle() const noexcept { return terminal_fd_; }

  private:
    int terminal_fd_ = -1;
    termios original_{};
    bool active_ = false;
};
