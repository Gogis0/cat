#include "stdio.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#define bufsiz (128 * 1024)


char *buffer;

int main() {
    int page_size = sysconf(_SC_PAGESIZE);
    struct stat fs;
    stat(0, &fs);
    
    int insize = bufsiz;
    if (fs.st_blksize > insize) insize = fs.st_blksize;


    buffer = malloc(insize+ page_size - 1);
    int pocet;
    do {
        pocet = read(0, buffer, bufsiz);
        write(1, buffer, pocet);
    } while (pocet > 0);
}
