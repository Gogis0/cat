#include "unistd.h"
#include "stdio.h"
#include <sys/stat.h>
#include <pthread.h>

#define bufsiz (1 * 1024 * 1024)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mam_zapisane, mam_precitane;

char buffer[bufsiz];
int pocet = 0;

void *citaj(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("reader\n");
        pocet = read(0, buffer, bufsiz);
        pthread_cond_signal(&mam_precitane);
    
        if (pocet == 0) {
            pocet = -1;
            puts("KONEEEEEEEEEEEEEEEEEEC");
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        } else {
            printf("mam precitane, idem spat\n");
            pthread_cond_wait(&mam_zapisane, &mutex);
        }
    }
}

void *pis(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("writer\n");
        if (pocet == -1) {
            pthread_exit(NULL);
        }

        //write(1, buffer, pocet);
        pthread_cond_signal(&mam_zapisane);
        printf("dal som signal, ze mam zapisane\n");
        pthread_cond_wait(&mam_precitane, &mutex);
        printf("precooo sa tu nic nedeje\n");
    }
}

int main() {
    pthread_t reader, writer;
    pthread_create(&reader, NULL, citaj, NULL);
    pthread_create(&writer, NULL, pis, NULL);
    /*
    (void) pthread_join(reader, NULL);
    (void) pthread_join(writer, NULL);
    puts("return 0l");
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&mam_zapisane);
    pthread_cond_destroy(&mam_precitane);
    pthread_exit(NULL);
    */
    sleep(4);
    return 0;
}
