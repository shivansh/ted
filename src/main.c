#include "editor.h"
#include "terminal.h"

int main() {
    enable_raw_mode();

    while (1) {
        process_keypress();
    }

    return 0;
}
