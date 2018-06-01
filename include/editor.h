#ifndef EDITOR_H_
#define EDITOR_H_

#include <termios.h>

struct editor_config {
    int screenrows;
    int screencols;
    struct termios orig_termios;
};

struct editor_config E;

char read_key();
void process_keypress();
void disable_raw_mode();
void enable_raw_mode();
int get_window_size(int*, int*);
void init_editor();

#endif  // EDITOR_H_
