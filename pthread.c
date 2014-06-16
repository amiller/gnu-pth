/*
**  GNU Pth - The GNU Portable Threads
**  Copyright (c) 1999-2006 Ralf S. Engelschall <rse@engelschall.com>
**
**  This file is part of GNU Pth, a non-preemptive thread scheduling
**  library which can be found at http://www.gnu.org/software/pth/.
**
**  This library is free software; you can redistribute it and/or
**  modify it under the terms of the GNU Lesser General Public
**  License as published by the Free Software Foundation; either
**  version 2.1 of the License, or (at your option) any later version.
**
**  This library is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**  Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License along with this library; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
**  USA, or contact Ralf S. Engelschall <rse@engelschall.com>.
**
**  pthread.c: POSIX Thread ("Pthread") API for Pth
*/
                             /* ``The nice thing about standards is that
                                  there are so many to choose from.  And if
                                  you really don't like all the standards you
                                  just have to wait another year until the one
                                  arises you are looking for'' 
                                                 -- Tannenbaum, 'Introduction
                                                    to Computer Networks' */

/*
**  HEADER STUFF
*/

/*
 * Include our own Pthread and then the private Pth header.
 * The order here is very important to get correct namespace hiding!
 */
#define _PTHREAD_PRIVATE
#include "pthread_p.h"
#include "pth_p.h"
#undef _PTHREAD_PRIVATE

/* general success return value */
#ifdef OK
#undef OK
#endif
#define OK 0

/*
**  GLOBAL STUFF
*/

static void pth_pthread_shutdown(void)
{
    pth_kill();
    return;
}

static int pth_pthread_initialized = FALSE;

#define pth_pthread_initialize() \
    do { \
        if (pth_pthread_initialized == FALSE) { \
            pth_pthread_initialized = TRUE; \
            pth_init(); \
            atexit(pth_pthread_shutdown); \
        } \
    } while (0)

/*
**  THREAD ATTRIBUTE ROUTINES
*/

int pth_pthread_attr_init(pthread_attr_t *attr)
{
    pth_attr_t na;
    pth_debug1("pth_pthread_attr_init:");

    pth_pthread_initialize();
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    if ((na = pth_attr_new()) == NULL)
        return errno;
    (*attr) = (pthread_attr_t)na;
    return OK;
}

int pth_pthread_attr_destroy(pthread_attr_t *attr)
{
    pth_attr_t na;

    if (attr == NULL || *attr == NULL)
        return pth_error(EINVAL, EINVAL);
    na = (pth_attr_t)(*attr);
    pth_attr_destroy(na);
    *attr = NULL;
    return OK;
}

int pth_pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched)
{
    if (attr == NULL || inheritsched == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *schedparam)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *schedparam)
{
    if (attr == NULL || schedparam == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_setschedpolicy(pthread_attr_t *attr, int schedpolicy)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *schedpolicy)
{
    if (attr == NULL || schedpolicy == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_getscope(const pthread_attr_t *attr, int *scope)
{
    if (attr == NULL || scope == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_set((pth_attr_t)(*attr), PTH_ATTR_STACK_SIZE, (unsigned int)stacksize))
        return errno;
    return OK;
}

int pth_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
    if (attr == NULL || stacksize == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_get((pth_attr_t)(*attr), PTH_ATTR_STACK_SIZE, (unsigned int *)stacksize))
        return errno;
    return OK;
}

int pth_pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_set((pth_attr_t)(*attr), PTH_ATTR_STACK_ADDR, (char *)stackaddr))
        return errno;
    return OK;
}

int pth_pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr)
{
    if (attr == NULL || stackaddr == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_get((pth_attr_t)(*attr), PTH_ATTR_STACK_ADDR, (char **)stackaddr))
        return errno;
    return OK;
}

int pth_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
    int s;

    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    if (detachstate == PTHREAD_CREATE_DETACHED)
        s = FALSE;
    else  if (detachstate == PTHREAD_CREATE_JOINABLE)
        s = TRUE;
    else
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_set((pth_attr_t)(*attr), PTH_ATTR_JOINABLE, s))
        return errno;
    return OK;
}

int pth_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
    int s;

    if (attr == NULL || detachstate == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_get((pth_attr_t)(*attr), PTH_ATTR_JOINABLE, &s))
        return errno;
    if (s == TRUE)
        *detachstate = PTHREAD_CREATE_JOINABLE;
    else
        *detachstate = PTHREAD_CREATE_DETACHED;
    return OK;
}

int pth_pthread_attr_setguardsize(pthread_attr_t *attr, int stacksize)
{
    if (attr == NULL || stacksize < 0)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_getguardsize(const pthread_attr_t *attr, int *stacksize)
{
    if (attr == NULL || stacksize == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_attr_setname_np(pthread_attr_t *attr, char *name)
{
    if (attr == NULL || name == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_set((pth_attr_t)(*attr), PTH_ATTR_NAME, name))
        return errno;
    return OK;
}

int pth_pthread_attr_getname_np(const pthread_attr_t *attr, char **name)
{
    if (attr == NULL || name == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_get((pth_attr_t)(*attr), PTH_ATTR_NAME, name))
        return errno;
    return OK;
}

int pth_pthread_attr_setprio_np(pthread_attr_t *attr, int prio)
{
    if (attr == NULL || (prio < PTH_PRIO_MIN || prio > PTH_PRIO_MAX))
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_set((pth_attr_t)(*attr), PTH_ATTR_PRIO, prio))
        return errno;
    return OK;
}

int pth_pthread_attr_getprio_np(const pthread_attr_t *attr, int *prio)
{
    if (attr == NULL || prio == NULL)
        return pth_error(EINVAL, EINVAL);
    if (!pth_attr_get((pth_attr_t)(*attr), PTH_ATTR_PRIO, prio))
        return errno;
    return OK;
}

/*
**  THREAD ROUTINES
*/

int pth_pthread_create(
    pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine)(void *), void *arg)
{
    pth_attr_t na;

    pth_pthread_initialize();
    if (thread == NULL || start_routine == NULL)
        return pth_error(EINVAL, EINVAL);
    if (pth_ctrl(PTH_CTRL_GETTHREADS) >= PTHREAD_THREADS_MAX)
        return pth_error(EAGAIN, EAGAIN);
    if (attr != NULL)
        na = (pth_attr_t)(*attr);
    else
        na = PTH_ATTR_DEFAULT;
    *thread = (pthread_t)pth_spawn(na, start_routine, arg);
    if (*thread == NULL)
        return pth_error(EAGAIN, EAGAIN);
    return OK;
}

int __pth_pthread_detach(pthread_t thread)
{
    pth_attr_t na;

    if (thread == NULL)
        return pth_error(EINVAL, EINVAL);
    if ((na = pth_attr_of((pth_t)thread)) == NULL)
        return errno;
    if (!pth_attr_set(na, PTH_ATTR_JOINABLE, FALSE))
        return errno;
    pth_attr_destroy(na);
    return OK;
}

pthread_t pth_pthread_self(void)
{
    pth_pthread_initialize();
    return (pthread_t)pth_self();
}

int pth_pthread_equal(pthread_t t1, pthread_t t2)
{
    return (t1 == t2);
}

int pth_pthread_yield_np(void)
{
    pth_pthread_initialize();
    pth_yield(NULL);
    return OK;
}

void pth_pthread_exit(void *value_ptr)
{
    pth_pthread_initialize();
    pth_exit(value_ptr);
    return;
}

int pth_pthread_join(pthread_t thread, void **value_ptr)
{
    pth_pthread_initialize();
    if (!pth_join((pth_t)thread, value_ptr))
        return errno;
    if (value_ptr != NULL)
        if (*value_ptr == PTH_CANCELED)
            *value_ptr = PTHREAD_CANCELED;
    return OK;
}

int pth_pthread_once(
    pthread_once_t *once_control, void (*init_routine)(void))
{
    pth_pthread_initialize();
    if (once_control == NULL || init_routine == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*once_control != 1)
        init_routine();
    *once_control = 1;
    return OK;
}

int pth_pthread_sigmask(int how, const sigset_t *set, sigset_t *oset)
{
    pth_pthread_initialize();
    return pth_sigmask(how, set, oset);
}

int pth_pthread_kill(pthread_t thread, int sig)
{
    if (!pth_raise((pth_t)thread, sig))
        return errno;
    return OK;
}

/*
**  CONCURRENCY ROUTINES
**
**  We just have to provide the interface, because SUSv2 says:
**  "The pth_pthread_setconcurrency() function allows an application to
**  inform the threads implementation of its desired concurrency
**  level, new_level. The actual level of concurrency provided by the
**  implementation as a result of this function call is unspecified."
*/

static int pth_pthread_concurrency = 0;

int pth_pthread_getconcurrency(void)
{
    return pth_pthread_concurrency;
}

int pth_pthread_setconcurrency(int new_level)
{
    if (new_level < 0)
        return pth_error(EINVAL, EINVAL);
    pth_pthread_concurrency = new_level;
    return OK;
}

/*
**  CONTEXT ROUTINES
*/

int pth_pthread_key_create(pthread_key_t *key, void (*destructor)(void *))
{
    pth_pthread_initialize();
    if (!pth_key_create((pth_key_t *)key, destructor))
        return errno;
    return OK;
}

int pth_pthread_key_delete(pthread_key_t key)
{
    if (!pth_key_delete((pth_key_t)key))
        return errno;
    return OK;
}

int pth_pthread_setspecific(pthread_key_t key, const void *value)
{
    if (!pth_key_setdata((pth_key_t)key, value))
        return errno;
    return OK;
}

void *pth_pthread_getspecific(pthread_key_t key)
{
    return pth_key_getdata((pth_key_t)key);
}

/*
**  CANCEL ROUTINES
*/

int pth_pthread_cancel(pthread_t thread)
{
    if (!pth_cancel((pth_t)thread))
        return errno;
    return OK;
}

void pthread_testcancel(void)
{
    pth_cancel_point();
    return;
}

int pth_pthread_setcancelstate(int state, int *oldstate)
{
    int s, os;

    if (oldstate != NULL) {
        pth_cancel_state(0, &os);
        if (os & PTH_CANCEL_ENABLE)
            *oldstate = PTHREAD_CANCEL_ENABLE;
        else
            *oldstate = PTHREAD_CANCEL_DISABLE;
    }
    if (state != 0) {
        pth_cancel_state(0, &s);
        if (state == PTHREAD_CANCEL_ENABLE) {
            s |= PTH_CANCEL_ENABLE;
            s &= ~(PTH_CANCEL_DISABLE);
        }
        else {
            s |= PTH_CANCEL_DISABLE;
            s &= ~(PTH_CANCEL_ENABLE);
        }
        pth_cancel_state(s, NULL);
    }
    return OK;
}

int pth_pthread_setcanceltype(int type, int *oldtype)
{
    int t, ot;

    if (oldtype != NULL) {
        pth_cancel_state(0, &ot);
        if (ot & PTH_CANCEL_DEFERRED)
            *oldtype = PTHREAD_CANCEL_DEFERRED;
        else
            *oldtype = PTHREAD_CANCEL_ASYNCHRONOUS;
    }
    if (type != 0) {
        pth_cancel_state(0, &t);
        if (type == PTHREAD_CANCEL_DEFERRED) {
            t |= PTH_CANCEL_DEFERRED;
            t &= ~(PTH_CANCEL_ASYNCHRONOUS);
        }
        else {
            t |= PTH_CANCEL_ASYNCHRONOUS;
            t &= ~(PTH_CANCEL_DEFERRED);
        }
        pth_cancel_state(t, NULL);
    }
    return OK;
}

/*
**  SCHEDULER ROUTINES
*/

int pth_pthread_setschedparam(pthread_t pthread, int policy, const struct sched_param *param)
{
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_getschedparam(pthread_t pthread, int *policy, struct sched_param *param)
{
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

/*
**  CLEANUP ROUTINES
*/

void pth_pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    pth_pthread_initialize();
    pth_cleanup_push(routine, arg);
    return;
}

void pth_pthread_cleanup_pop(int execute)
{
    pth_cleanup_pop(execute);
    return;
}

/*
**  AT-FORK SUPPORT
*/

struct pth_pthread_atfork_st {
    void (*prepare)(void);
    void (*parent)(void);
    void (*child)(void);
};
static struct pth_pthread_atfork_st pth_pthread_atfork_info[PTH_ATFORK_MAX];
static int pth_pthread_atfork_idx = 0;

static void pth_pthread_atfork_cb_prepare(void *_info)
{
    struct pth_pthread_atfork_st *info = (struct pth_pthread_atfork_st *)_info;
    info->prepare();
    return;
}
static void pth_pthread_atfork_cb_parent(void *_info)
{
    struct pth_pthread_atfork_st *info = (struct pth_pthread_atfork_st *)_info;
    info->parent();
    return;
}
static void pth_pthread_atfork_cb_child(void *_info)
{
    struct pth_pthread_atfork_st *info = (struct pth_pthread_atfork_st *)_info;
    info->child();
    return;
}

int pth_pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
    struct pth_pthread_atfork_st *info;

    if (pth_pthread_atfork_idx > PTH_ATFORK_MAX-1)
        return pth_error(ENOMEM, ENOMEM);
    info = &pth_pthread_atfork_info[pth_pthread_atfork_idx++];
    info->prepare = prepare;
    info->parent  = parent;
    info->child   = child;
    if (!pth_atfork_push(pth_pthread_atfork_cb_prepare,
                         pth_pthread_atfork_cb_parent,
                         pth_pthread_atfork_cb_child, info))
        return errno;
    return OK;
}

/*
**  MUTEX ATTRIBUTE ROUTINES
*/

int pth_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    pth_pthread_initialize();
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_getprioceiling(pthread_mutexattr_t *attr, int *prioceiling)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_getprotocol(pthread_mutexattr_t *attr, int *protocol)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_getpshared(pthread_mutexattr_t *attr, int *pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutexattr_gettype(pthread_mutexattr_t *attr, int *type)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

/*
**  MUTEX ROUTINES
*/

int pth_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    pth_mutex_t *m;

    pth_pthread_initialize();
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if ((m = (pth_mutex_t *)malloc(sizeof(pth_mutex_t))) == NULL)
        return errno;
    if (!pth_mutex_init(m))
        return errno;
    (*mutex) = (pthread_mutex_t)m;
    return OK;
}

int pth_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    free(*mutex);
    *mutex = NULL;
    return OK;
}

int pth_pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutex_getprioceiling(pthread_mutex_t *mutex, int *prioceiling)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_mutex_lock(pthread_mutex_t *mutex)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    if (!pth_mutex_acquire((pth_mutex_t *)(*mutex), FALSE, NULL))
        return errno;
    return OK;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    if (!pth_mutex_acquire((pth_mutex_t *)(*mutex), TRUE, NULL))
        return errno;
    return OK;
}

int pth_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    if (mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    if (!pth_mutex_release((pth_mutex_t *)(*mutex)))
        return errno;
    return OK;
}

/*
**  LOCK ATTRIBUTE ROUTINES
*/

int pth_pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    pth_pthread_initialize();
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

/*
**  LOCK ROUTINES
*/

int pth_pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
    pth_rwlock_t *rw;

    pth_pthread_initialize();
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if ((rw = (pth_rwlock_t *)malloc(sizeof(pth_rwlock_t))) == NULL)
        return errno;
    if (!pth_rwlock_init(rw))
        return errno;
    (*rwlock) = (pthread_rwlock_t)rw;
    return OK;
}

int pth_pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    free(*rwlock);
    *rwlock = NULL;
    return OK;
}

int pth_pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*rwlock == PTHREAD_RWLOCK_INITIALIZER)
        if (pth_pthread_rwlock_init(rwlock, NULL) != OK)
            return errno;
    if (!pth_rwlock_acquire((pth_rwlock_t *)(*rwlock), PTH_RWLOCK_RD, FALSE, NULL))
        return errno;
    return OK;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*rwlock == PTHREAD_RWLOCK_INITIALIZER)
        if (pth_pthread_rwlock_init(rwlock, NULL) != OK)
            return errno;
    if (!pth_rwlock_acquire((pth_rwlock_t *)(*rwlock), PTH_RWLOCK_RD, TRUE, NULL))
        return errno;
    return OK;
}

int pth_pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*rwlock == PTHREAD_RWLOCK_INITIALIZER)
        if (pth_pthread_rwlock_init(rwlock, NULL) != OK)
            return errno;
    if (!pth_rwlock_acquire((pth_rwlock_t *)(*rwlock), PTH_RWLOCK_RW, FALSE, NULL))
        return errno;
    return OK;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*rwlock == PTHREAD_RWLOCK_INITIALIZER)
        if (pth_pthread_rwlock_init(rwlock, NULL) != OK)
            return errno;
    if (!pth_rwlock_acquire((pth_rwlock_t *)(*rwlock), PTH_RWLOCK_RW, TRUE, NULL))
        return errno;
    return OK;
}

int pth_pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*rwlock == PTHREAD_RWLOCK_INITIALIZER)
        if (pth_pthread_rwlock_init(rwlock, NULL) != OK)
            return errno;
    if (!pth_rwlock_release((pth_rwlock_t *)(*rwlock)))
        return errno;
    return OK;
}

/*
**  CONDITION ATTRIBUTE ROUTINES
*/

int pth_pthread_condattr_init(pthread_condattr_t *attr)
{
    pth_pthread_initialize();
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_condattr_destroy(pthread_condattr_t *attr)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* nothing to do for us */
    return OK;
}

int pth_pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

int pth_pthread_condattr_getpshared(pthread_condattr_t *attr, int *pshared)
{
    if (attr == NULL)
        return pth_error(EINVAL, EINVAL);
    /* not supported */
    return pth_error(ENOSYS, ENOSYS);
}

/*
**  CONDITION ROUTINES
*/

int pth_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    pth_cond_t *cn;

    pth_pthread_initialize();
    if (cond == NULL)
        return pth_error(EINVAL, EINVAL);
    if ((cn = (pth_cond_t *)malloc(sizeof(pth_cond_t))) == NULL)
        return errno;
    if (!pth_cond_init(cn))
        return errno;
    (*cond) = (pthread_cond_t)cn;
    return OK;
}

int pth_pthread_cond_destroy(pthread_cond_t *cond)
{
    if (cond == NULL)
        return pth_error(EINVAL, EINVAL);
    free(*cond);
    *cond = NULL;
    return OK;
}

int pth_pthread_cond_broadcast(pthread_cond_t *cond)
{
    if (cond == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*cond == PTHREAD_COND_INITIALIZER)
        if (pth_pthread_cond_init(cond, NULL) != OK)
            return errno;
    if (!pth_cond_notify((pth_cond_t *)(*cond), TRUE))
        return errno;
    return OK;
}

int pth_pthread_cond_signal(pthread_cond_t *cond)
{
    if (cond == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*cond == PTHREAD_COND_INITIALIZER)
        if (pth_pthread_cond_init(cond, NULL) != OK)
            return errno;
    if (!pth_cond_notify((pth_cond_t *)(*cond), FALSE))
        return errno;
    return OK;
}

int pth_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    if (cond == NULL || mutex == NULL)
        return pth_error(EINVAL, EINVAL);
    if (*cond == PTHREAD_COND_INITIALIZER)
        if (pth_pthread_cond_init(cond, NULL) != OK)
            return errno;
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    if (!pth_cond_await((pth_cond_t *)(*cond), (pth_mutex_t *)(*mutex), NULL))
        return errno;
    return OK;
}

int pth_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           const struct timespec *abstime)
{
    pth_event_t ev;
    static pth_key_t ev_key = PTH_KEY_INIT;

    if (cond == NULL || mutex == NULL || abstime == NULL)
        return pth_error(EINVAL, EINVAL);
#ifdef __amigaos__
    if (abstime->ts_sec < 0 || abstime->ts_nsec < 0 || abstime->ts_nsec >= 1000000000)
#else
    if (abstime->tv_sec < 0 || abstime->tv_nsec < 0 || abstime->tv_nsec >= 1000000000)
#endif
        return pth_error(EINVAL, EINVAL);
    if (*cond == PTHREAD_COND_INITIALIZER)
        if (pth_pthread_cond_init(cond, NULL) != OK)
            return errno;
    if (*mutex == PTHREAD_MUTEX_INITIALIZER)
        if (pth_pthread_mutex_init(mutex, NULL) != OK)
            return errno;
    ev = pth_event(PTH_EVENT_TIME|PTH_MODE_STATIC, &ev_key,
#ifdef __amigaos__
                   pth_time(abstime->ts_sec, (abstime->ts_nsec)/1000)
#else
                   pth_time(abstime->tv_sec, (abstime->tv_nsec)/1000)
#endif
    );
    if (!pth_cond_await((pth_cond_t *)(*cond), (pth_mutex_t *)(*mutex), ev))
        return errno;
    if (pth_event_status(ev) == PTH_STATUS_OCCURRED)
        return ETIMEDOUT;
    return OK;
}

/*
**  POSIX 1003.1j
*/

int pth_pthread_abort(pthread_t thread)
{
    if (!pth_abort((pth_t)thread))
        return errno;
    return OK;
}

/*
**  THREAD-SAFE REPLACEMENT FUNCTIONS
*/

pid_t __pth_pthread_fork(void)
{
    pth_pthread_initialize();
    return pth_fork();
}

unsigned int __pth_pthread_sleep(unsigned int sec)
{
    pth_pthread_initialize();
    return pth_sleep(sec);
}

int __pth_pthread_system(const char *cmd)
{
    pth_pthread_initialize();
    return pth_system(cmd);
}

int __pth_pthread_nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    pth_pthread_initialize();
    return pth_nanosleep(rqtp, rmtp);
}

int __pth_pthread_usleep(unsigned int sec)
{
    pth_pthread_initialize();
    return pth_usleep(sec);
}

int __pth_pthread_sigwait(const sigset_t *set, int *sig)
{
    pth_pthread_initialize();
    return pth_sigwait(set, sig);
}

pid_t __pth_pthread_waitpid(pid_t pid, int *status, int options)
{
    pth_pthread_initialize();
    return pth_waitpid(pid, status, options);
}

int __pth_pthread_connect(int s, struct sockaddr *addr, socklen_t addrlen)
{
    pth_pthread_initialize();
    return pth_connect(s, addr, addrlen);
}

int __pth_pthread_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    pth_pthread_initialize();
    return pth_accept(s, addr, addrlen);
}

int __pth_pthread_select(int nfds, fd_set *readfds, fd_set *writefds,
                    fd_set *exceptfds, struct timeval *timeout)
{
    pth_pthread_initialize();
    return pth_select(nfds, readfds, writefds, exceptfds, timeout);
}

int __pth_pthread_poll(struct pollfd *pfd, nfds_t nfd, int timeout)
{
    pth_pthread_initialize();
    return pth_poll(pfd, nfd, timeout);
}

ssize_t __pth_pthread_read(int fd, void *buf, size_t nbytes)
{
    pth_pthread_initialize();
    return pth_read(fd, buf, nbytes);
}

ssize_t __pth_pthread_write(int fd, const void *buf, size_t nbytes)
{
    pth_pthread_initialize();
    return pth_write(fd, buf, nbytes);
}

ssize_t __pth_pthread_readv(int fd, const struct iovec *piovec, int iocnt)
{
    pth_pthread_initialize();
    return pth_readv(fd, piovec, iocnt);
}

ssize_t __pth_pthread_writev(int fd, const struct iovec *piovec, int iocnt)
{
    pth_pthread_initialize();
    return pth_writev(fd, piovec, iocnt);
}

ssize_t __pth_pthread_recv(int fd, void *buf, size_t nbytes, int flags)
{
    pth_pthread_initialize();
    return pth_recv(fd, buf, nbytes, flags);
}

ssize_t __pth_pthread_send(int fd, const void *buf, size_t nbytes, int flags)
{
    pth_pthread_initialize();
    return pth_send(fd, buf, nbytes, flags);
}

ssize_t __pth_pthread_recvfrom(int fd, void *buf, size_t nbytes, int flags, struct sockaddr *from, socklen_t *fromlen)
{
    pth_pthread_initialize();
    return pth_recvfrom(fd, buf, nbytes, flags, from, fromlen);
}

ssize_t __pth_pthread_sendto(int fd, const void *buf, size_t nbytes, int flags, const struct sockaddr *to, socklen_t tolen)
{
    pth_pthread_initialize();
    return pth_sendto(fd, buf, nbytes, flags, to, tolen);
}

ssize_t __pth_pthread_pread(int fd, void *buf, size_t nbytes, off_t offset)
{
    pth_pthread_initialize();
    return pth_pread(fd, buf, nbytes, offset);
}

ssize_t __pth_pthread_pwrite(int fd, const void *buf, size_t nbytes, off_t offset)
{
    pth_pthread_initialize();
    return pth_pwrite(fd, buf, nbytes, offset);
}

