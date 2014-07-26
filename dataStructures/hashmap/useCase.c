#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "DMap.h"
#include "MBuf.h"
#include "errors.h"

int main(int argc, char *argv[]) {
    DMap *dm = NULL;

    FILE *ifp = stdin;

    ULInt i_mBuf=0;
    unsigned int prevRead=0, MBUF_BLK_SZ=sysconf(_SC_PAGE_SIZE);
    unsigned long long int res=0;

    char c;
    struct MBuf *mb = freshMBuf(MBUF_BLK_SZ);
    while ((c = getc(ifp)) != EOF) {
        if (isdigit(c)) {
            res = (res * 10) + (c - '0');
            prevRead = 1;
        }
        else if (prevRead) {
            prevRead = 0;
            if (i_mBuf >= mb->size) {
                mb = resizeMBuf(mb, mb->size + MBUF_BLK_SZ);
                _fillBufWithContent(mb->buf + i_mBuf, mb->buf+mb->size, twoCharFill);
            }

            dm = pushDMap(dm, *(mb->buf + i_mBuf), res, 1);
            ++i_mBuf;
            res = 0;
            printf("#%ld read\n", i_mBuf);
        }
    }

    printf("dmSize: %ld\n", getSize(dm));
    free(mb); // Note: Won't be invoking destroyMBuf since memory in buf is freed during destroyDMap
    dm = destroyDMap(dm);
};
