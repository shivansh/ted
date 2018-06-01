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
    for (int y = 0; y < E.screenrows; y++) {
        buf_append(buf, "~\r\n", 3);
    }
    buf_append(buf, "\x1b[H", 3);
}

// refresh_screen prints out the escape sequence: Erase In Display.
// https://vt100.net/docs/vt100-ug/chapter3.html#ED
void refresh_screen() {
    struct buffer buf = BUF_INIT;
    clear_screen(&buf);
    draw_rows(&buf);
    write(STDOUT_FILENO, buf.b, buf.len);
    buf_free(&buf);
}
