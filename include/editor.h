#ifndef EDITOR_H_
#define EDITOR_H_

#include <termios.h>

typedef struct editor_row {
    int size;
    char *chars;
} erow;

struct editor_config {
    int cx, cy;  // cursor coordinates
    int screenrows;
    int screencols;
    erow *rows;
    struct termios orig_termios;
};

struct editor_config E;

char read_key();
void move_cursor(char);
void process_keypress();
void disable_raw_mode();
void enable_raw_mode();
int get_window_size(int *, int *);
void init_editor();

#endif  // EDITOR_H_
