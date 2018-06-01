#include "display.h"
#include "editor.h"

int main() {
    enable_raw_mode();
    init_editor();

    while (1) {
        refresh_screen();
        process_keypress();
    }

    return 0;
}
