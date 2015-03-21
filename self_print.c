// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>

int main() {
    FILE *ifp = fopen(__FILE__, "r");
    if (ifp == NULL)
        return -1;
    char c;
    while ((c = getc(ifp)) != EOF) putchar(c);
    fclose(ifp);
}
