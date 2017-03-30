#include "unistd.h"
#include "stdio.h"
#include <sys/stat.h>

#define bufsiz (128 * 1024)

char buffer[bufsiz];

int main() {
    int pocet;
    do {
        pocet = read(0, buffer, bufsiz);
        write(1, buffer, pocet);
    } while(pocet > 0);
}
