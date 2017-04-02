#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

char *buffer;

int main() {
    int page_size = sysconf(_SC_PAGESIZE);
        
    int pocet = 0;
    buffer = malloc(page_size * 32);
    do {
        pocet = read(0, buffer, page_size * 32);
        write(1, buffer, pocet);
    } while (pocet > 0);
}
