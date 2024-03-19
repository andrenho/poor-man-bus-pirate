#include <cstdio>
#include "getline.hh"

size_t getline(char* buf, size_t max_sz)
{
    size_t pos = 0;

    for (;;) {

        char c = getchar();

        if (c >= 32 && c < 127 && pos < max_sz) {
            printf("%c", c);
            buf[pos++] = c;
        } else if ((c == 127 || c == '\b') && pos > 0) {
            printf("\e[1D \e[1D");
            buf[pos] = 0;
            --pos;
        } else if (c == '\n' || c == '\r') {
            puts("");
            buf[pos] = 0;
            return pos;
        }

    }
}