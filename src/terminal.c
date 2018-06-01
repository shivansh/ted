#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "error.h"
#include "terminal.h"

struct termios orig_termios;

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}

// enable_raw_mode enables raw mode in the terminal, where each keypress is
// processed as it comes in. This is unlike the default canonical (cooked) mode
// in which the terminal starts.
void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("tcgetattr");
    }
    atexit(disable_raw_mode);  // disable raw mode on exit

    struct termios raw = orig_termios;
    tcgetattr(STDIN_FILENO, &raw);
    // Turn off echo and canonical mode, enabling the input to be read
    // byte-by-byte instead of line-by-line. Also, disable SIGINT and SIGSTP.
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // By default, each newline '\n' is translated to "\r\n". Disable this
    // output processing.
    raw.c_oflag &= ~(OPOST);

    // Set character size to 8 bits per byte.
    raw.c_cflag |= (CS8);

    // Disable flow control on output.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    // Minimum number of bytes of input needed before read() can return.
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 10;  // tenths of a second

    // Wait for all pending output to be written to the terminal, and discard
    // any input that hasn't been read.
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}
