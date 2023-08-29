/* KallistiOS ##version##

   pthread_rwlock_timedrdlock.c
   Copyright (C) 2023 Lawrence Sebald

*/

#include "pthread-internal.h"
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <kos/rwsem.h>

int pthread_rwlock_timedrdlock(pthread_rwlock_t *__RESTRICT rwlock,
                               const struct timespec *__RESTRICT abstime) {
    int old, rv = 0;
    int tmo;
    struct timeval ctv;

    if(!rwlock || !abstime)
        return EFAULT;

    if(abstime->tv_nsec < 0 || abstime->tv_nsec > 1000000000L)
        return EINVAL;

    /* First, try to lock the lock before doing the hard work of figuring out
       the timing... POSIX says that if the lock can be acquired immediately
       then this function should never return a timeout, regardless of what
       abstime says. */
    old = errno;

    if(!rwsem_read_trylock(&rwlock->rwsem))
        return 0;

    /* Figure out the timeout we need to provide. */
    gettimeofday(&ctv, NULL);

    tmo = abstime->tv_sec - ctv.tv_sec;
    tmo += abstime->tv_nsec / (1000 * 1000) - ctv.tv_usec / 1000;

    if(tmo <= 0)
        return ETIMEDOUT;

    if(rwsem_read_lock_timed(&rwlock->rwsem, tmo))
        rv = errno;

    errno = old;
    return rv;
}
