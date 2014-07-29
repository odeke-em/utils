#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "DMap.h"
#include "MBuf.h"
#include "errors.h"

int main(int argc, char *argv[]) {
    // Goal here is given an arbitrary stream of integers,
    // take out duplicates however, maintain the set order
    DMap *dm = NULL;
    unsigned long long int n = 1000000, i, rNum, MAX_NUM = 578918283839281739, MIN_NUM=0;
    if (argc >= 2) {
        if (sscanf(argv[1], "%lld", &n) != 1)
            n = 1000000;
        if (argc >= 3) {
            if (sscanf(argv[2], "%lld", &MAX_NUM) != 1)
                MAX_NUM = 78919127392918;
            if (argc >= 4) {
                if (sscanf(argv[3], "%lld", &MIN_NUM) != 1)
                    MIN_NUM = 1;
            }
        }
    }

    printf("argc: %d c: %lld min: %lld max: %lld\n", argc, n, MIN_NUM, MAX_NUM);

    unsigned long long int i_mBuf=0;
    unsigned int prevRead=0, MBUF_BLK_SZ=sysconf(_SC_PAGE_SIZE);

    struct MBuf *mb = freshMBuf(MBUF_BLK_SZ);
    for (i=0; i < n; ++i) {
            if (i_mBuf >= mb->size) {
                printf("%lld/%lld\r", i_mBuf, n);
                mb = resizeMBuf(mb, mb->size + MBUF_BLK_SZ);
                _fillBufWithContent(mb->buf + i_mBuf, mb->buf+mb->size, twoCharFill);
            }

            while ((rNum = rand() * MAX_NUM) < MIN_NUM && rNum >= MAX_NUM) {
                printf("ici\n");
            }

            dm = pushDMap(dm, *(mb->buf + i_mBuf), rNum, 1);
            ++i_mBuf;
    }

    printf("\ndmSize: %ld\n", getSize(dm));
    free(mb); // Note: Won't be invoking destroyMBuf since memory in buf is freed during destroyDMap
    dm = destroyDMap(dm);
};
