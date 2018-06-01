#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "display.h"
#include "editor.h"
#include "error.h"

// Simulate CTRL key behavior by setting the most significant 3 bits of the
// character to 0.
#define CTRL_KEY(k) ((k)&0x1f)

char read_key() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1) {
            die("read");
        }
    }
    return c;
}

// move_cursor updates cursor coordinates based on the key pressed.
void move_cursor(char key) {
    switch (key) {
        case 'h':
            if (E.cx != 0) {
                E.cx--;
            }
            break;
        case 'l':
            if (E.cx != E.screencols) {
                E.cx++;
            }
            break;
        case 'k':
            if (E.cy != 0) {
                E.cy--;
            }
            break;
        case 'j':
            if (E.cy != E.screenrows) {
                E.cy++;
            }
            break;
    }
}

void process_keypress() {
    char c = read_key();
    switch (c) {
        case CTRL_KEY('q'):
            clear_screen();
            exit(0);

        case 'h':
        case 'l':
        case 'k':
        case 'j':
            move_cursor(c);
            break;

#ifdef DEBUG
        case iscntrl(c):
            // Avoid printing control characters.
            // Since output processing is disabled, '\n' is not automatically
            // translated to "\r\n".
            printf("%d\r\n", c);

        default:
            printf("%d ('%c')\r\n", c, c);
#endif
    }
}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
        die("tcsetattr");
    }
}

// enable_raw_mode enables raw mode in the terminal, where each keypress is
// processed as it comes in. This is unlike the default canonical (cooked) mode
// in which the terminal starts.
void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
        die("tcgetattr");
    }
    atexit(disable_raw_mode);  // disable raw mode on exit

    struct termios raw = E.orig_termios;
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

// get_window_size evaluates the number of rows and columns in the screen.
int get_window_size(int *rows, int *cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    }
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
}

// init_editor initializes editor state.
void init_editor() {
    E.cx = 0;
    E.cy = 0;

    if (get_window_size(&E.screenrows, &E.screencols) == -1) {
        die("get_window_size");
    }
}
