#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rwlock.c"
// #include "rwlock_wr.c"
// #include "mutex.c"
// #include "empty.c"

#include "barrier.c"

volatile int flag = 0;

volatile int waiting_1 = 0;
volatile int waiting_2 = 0;

uint64_t count[NUM_THREADS - 1];

void *worker(void *arg) {
    uint64_t id = (uint64_t)arg;
    barrier(&waiting_1, NUM_THREADS);

    uint64_t n = 0;
    while (flag == 0) {
        do_lock();
        ++n;
    }
    count[id] = n;

    barrier(&waiting_2, NUM_THREADS);

    return NULL;
}

void *timer(void *arg) {
    barrier(&waiting_1, NUM_THREADS);

    sleep(180);
    flag = 1;

    barrier(&waiting_2, NUM_THREADS);
    for (int i = 0; i < NUM_THREADS - 1; ++i) {
        printf("%lu\n", count[i]);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    for (uint64_t i = 0; i < NUM_THREADS - 1; +=i) {
        pthread_t th;
        pthread_create(&th, NULL, worker, (void *)i);
        pthread_detach(th);
    }

    pthread_t th;
    pthread_create(&th, NULL, timer, NULL);
    pthread_join(th, NULL);

    return 0;
}