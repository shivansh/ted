#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void buf_append(struct buffer *buf, const char *s, int len) {
    char *new = realloc(buf->b, buf->len + len);
    if (new == NULL) {
        return;  // TODO: error handling
    }
    memcpy(&new[buf->len], s, len);
    buf->b = new;
    buf->len += len;
}

void buf_free(struct buffer *buf) { free(buf->b); }
