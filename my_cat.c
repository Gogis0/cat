#include "unistd.h"
#include "stdio.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond;

const int bufsiz_const = 32;
int bufsiz;
char *buffer;
int pocet = 0;
int turn = 0;

void *citaj(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (turn == 1) pthread_cond_wait(&cond, &mutex);
        pocet = read(0, buffer, bufsiz);
    
        if (pocet == 0) {
            pocet = -1;
            turn = 1;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        turn = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

void *pis(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (turn == 0) pthread_cond_wait(&cond, &mutex);

        if (pocet == -1) {
            pthread_exit(NULL);
        }
        write(1, buffer, pocet);
        turn = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    int page_size =  sysconf(_SC_PAGESIZE); 
    bufsiz = page_size * bufsiz_const;
    buffer = malloc(bufsiz);

    pthread_t reader, writer;
    pthread_create(&reader, NULL, citaj, NULL);
    pthread_create(&writer, NULL, pis, NULL);
    
    (void) pthread_join(reader, NULL);
    (void) pthread_join(writer, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}
