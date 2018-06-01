#ifndef BUFFER_H_
#define BUFFER_H_

struct buffer {
    char *b;
    int len;
};

#define BUF_INIT \
    { NULL, 0 }

void buf_append(struct buffer *, const char *, int);
void buf_free(struct buffer *);

#endif  // BUFFER_H_
