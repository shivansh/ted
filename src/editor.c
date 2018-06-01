#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"
#include "terminal.h"

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

void process_keypress() {
    char c = read_key();
    switch (c) {
        case CTRL_KEY('q'):
            exit(0);

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
