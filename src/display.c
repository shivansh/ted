#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "display.h"
#include "editor.h"

void clear_screen() {
    struct buffer buf = BUF_INIT;
    buf_append(&buf, "\x1b[2J", 4);
    // Reposition the cursor to the top-left corner of the screen.
    buf_append(&buf, "\x1b[H", 3);
    write(STDOUT_FILENO, buf.b, buf.len);
    buf_free(&buf);
}

void draw_rows(struct buffer *buf) {
    // Clear the current row before redrawing it.
    buf_append(buf, "\x1b[K", 4);
    for (int y = 0; y < E.screenrows; y++) {
        buf_append(buf, "~\r\n", 3);
    }
}

// refresh_screen prints out the escape sequence: Erase In Display.
// https://vt100.net/docs/vt100-ug/chapter3.html#ED
void refresh_screen() {
    struct buffer buf = BUF_INIT;
    // Hide the cursor before redrawing the screen.
    buf_append(&buf, "\x1b[?25l", 6);

    // Start rendering after positioning the cursor to the top-left.
    draw_rows(&buf);

    // Reposition the cursor to the top-left corner, while converting 0-indexed
    // offsets to 1-indexed for tracking cursor coordinates.
    char b[32];
    snprintf(b, sizeof(b), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    buf_append(&buf, b, strlen(b));

    // Render the cursor back.
    buf_append(&buf, "\x1b[?25h", 6);

    write(STDOUT_FILENO, buf.b, buf.len);
    buf_free(&buf);
}
