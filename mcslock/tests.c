#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <stdio.h>

#define TEST_MCSLOCK

#ifdef TEST_MCSLOCK
#include "mcslock.h"
#define lock_t mcslock_t
#define lock_init(lock) mcslock_init(lock)
#define lock_acquire(lock, node) mcslock_acquire(lock, node)
#define lock_release(lock, node) mcslock_release(lock, node)
#else
#include "ticketlock.h"
#define lock_t ticket_lock_t
#define lock_init(lock) ticket_init(lock)
#define lock_acquire(lock, node) ticket_acquire(lock)
#define lock_release(lock, node) ticket_release(lock)
#endif

#define TEST_LOOP 100

static int shared_num = 0;

static void *thread(void *arg)
{
    lock_t *lock = (lock_t *) arg;
#ifdef TEST_MCSLOCK
    mcsnode_t node;
#endif
    struct timespec t = (struct timespec) {
        .tv_sec = 0,
        .tv_nsec = 10000,
    };

    for (int i = 0; i < TEST_LOOP; i++)
    {
        lock_acquire(lock, &node);
        nanosleep(&t, NULL);
        shared_num += 1;
        lock_release(lock, &node);
    }

    return NULL;
}

int main(void)
{
    int nprocs = get_nprocs();
    if (nprocs < 0)
        return -1;

    printf("nprocs = %d\n", nprocs);

    pthread_t *pid = calloc(nprocs, sizeof(pthread_t));

    lock_t lock;

    for (int i = 0; i < nprocs; i++)
        pthread_create(&pid[i], NULL, thread, &lock);


    for (int i = 0; i < nprocs; i++)
        pthread_join(pid[i], NULL);

    free(pid);
    return 0;
}
