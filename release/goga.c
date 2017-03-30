#include "unistd.h"
#include "stdio.h"
#include <sys/stat.h>
#include <pthread.h>

#define bufsiz (1 * 1024 * 1024)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char buffer[bufsiz];
int pocet = -1;

void *citaj(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        pocet = read(0, buffer, bufsiz);
        pthread_mutex_unlock(&mutex);
        if (pocet == 0) return 0;
        sleep(1);
    }
}

void *pis(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (pocet == 0) return 0;
        write(1, buffer, pocet);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main() {
    pthread_t reader, writer;
    pthread_create(&reader, NULL, citaj, NULL);
    pthread_create(&writer, NULL, pis, NULL);

    (void) pthread_join(reader, NULL);
    (void) pthread_join(writer, NULL);
    return 0;
}
