#include <stdio.h>
#include <stdlib.h>

#include "display.h"

void die(const char *s) {
    clear_screen();
    perror(s);
    exit(1);
}
