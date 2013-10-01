/*
 * Copyright (c) 2012-2013 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#include <vx_internal.h>

#define BILLION (1000000000)

vx_bool vxCreateSem(vx_sem_t *sem, vx_uint32 count)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    int ret = sem_init(sem, 0, count);
    VX_PRINT(VX_ZONE_OSAL, "sem_init(%p,%u)=>%d errno=%d\n",sem,count,ret,errno);
    if (ret == 0)
#elif defined(DARWIN)
    char name[255];
    sprintf(name, "sem-%p", sem);
    sem_unlink(name);
    *sem = sem_open(name, O_CREAT, 0700, count);
    VX_PRINT(VX_ZONE_OSAL, "sem_open(%s, %d[%d])=>%p (%p) errno=%d\n",name,count,SEM_VALUE_MAX,*sem,SEM_FAILED,errno);
    if (*sem != SEM_FAILED)
#elif defined(WIN32) || defined(UNDER_CE)
    *sem = CreateSemaphore(NULL, count, count, NULL);
    if (*sem)
#endif
        return vx_true_e;
    else
        return vx_false_e;
}

void vxDestroySem(vx_sem_t *sem)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    sem_destroy(sem);
#elif defined(DARWIN)
    char name[255];
    sprintf(name, "sem-%p", sem);
    sem_close(*sem);
    sem_unlink(name);
#elif defined(WIN32) || defined(UNDER_CE)
    CloseHandle(sem);
#endif
}

vx_bool vxSemPost(vx_sem_t *sem)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    int ret = sem_post(sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_post(%p)=>%d errno=%d\n", sem, ret, errno);
    if (ret == 0)
#elif defined(DARWIN)
    int ret = sem_post(*sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_post(%p)=>%d errno=%d\n", *sem, ret, errno);
    if (ret == 0)
#elif defined(WIN32) || defined(UNDER_CE)
    if (ReleaseSemaphore(*sem, 1, NULL) == TRUE)
#endif
        return vx_true_e;
    else
        return vx_false_e;
}

vx_bool vxSemWait(vx_sem_t *sem)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    int ret = sem_wait(sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_wait(%p)=>%d errno=%d\n",sem,ret,errno);
    if (ret == 0)
#elif defined(DARWIN)
    int ret = sem_wait(*sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_wait(%p)=>%d errno=%d\n",*sem,ret,errno);
    if (ret == 0)
#elif defined(WIN32) || defined(UNDER_CE)
    if (WaitForSingleObject(*sem, INFINITE) == WAIT_OBJECT_0)
#endif
        return vx_true_e;
    else
        return vx_false_e;
}

vx_bool vxSemTryWait(vx_sem_t *sem)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    int ret = sem_trywait(sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_trywait(%p)=>%d errno=%d\n",sem,ret,errno);
    if (ret == 0)
#elif defined(DARWIN)
    int value;
    int ret = 0;
    ret = sem_getvalue(*sem, &value);
    ret = sem_trywait(*sem);
    VX_PRINT(VX_ZONE_OSAL, "sem_trywait(%p)=>%d value=%d errno=%d\n",*sem,ret,value,errno);
    if (ret == 0)
#elif defined(WIN32) || defined(UNDER_CE)
    if (WaitForSingleObject(*sem, 0) == WAIT_OBJECT_0)
#endif
        return vx_true_e;
    else
        return vx_false_e;
}

vx_bool vxDeinitEvent(vx_event_t *e)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    int err = 0;
    do {
        err = pthread_cond_destroy(&e->cond);
        if (err == EBUSY) {
            pthread_mutex_lock(&e->mutex);
            e->set = vx_false_e;
            pthread_cond_broadcast(&e->cond);
            pthread_mutex_unlock(&e->mutex);
        }
    } while (err != 0);
    pthread_condattr_destroy(&e->attr);
    pthread_mutex_destroy(&e->mutex);
    return vx_true_e;
#elif defined(WIN32) || defined(UNDER_CE)
    return CloseHandle(*e);
#endif
}

vx_bool vxInitEvent(vx_event_t *e, vx_bool autoreset)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    int err = 0;
    err |= pthread_mutex_init(&e->mutex, NULL);
    err |= pthread_condattr_init(&e->attr);
    err |= pthread_cond_init(&e->cond, &e->attr);
    e->set = vx_false_e;
    e->autoreset = autoreset;
    if (err == 0)
#elif defined(WIN32) || defined(UNDER_CE)
    BOOL manual = TRUE;
    if (autoreset)
        manual = FALSE;
    *e = CreateEvent(NULL, manual, FALSE, NULL);
    if (*e != NULL)
#endif
        return vx_true_e;
    else
        return vx_false_e;
}

#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
vx_bool vxWaitEventInternal(vx_event_t *e, vx_uint32 ms)
{
    int retcode = 0;
    vx_bool ret = vx_false_e;
    if (ms < VX_INT_FOREVER)
    {
        struct timespec time_spec;
        struct timeval now;
        gettimeofday(&now, NULL);
        time_spec.tv_sec = now.tv_sec + (ms / 1000);
        time_spec.tv_nsec = (now.tv_usec * 1000) + ((ms%1000) * 1000000);
        if (time_spec.tv_nsec > BILLION) {
            time_spec.tv_sec += 1;
            time_spec.tv_nsec -= BILLION;
        }
        retcode = pthread_cond_timedwait(&e->cond, &e->mutex, &time_spec);
    }
    else
        retcode = pthread_cond_wait(&e->cond, &e->mutex);
    if (retcode == ETIMEDOUT && e->set == vx_false_e)
        ret = vx_false_e;
    else if (retcode == ETIMEDOUT && e->set == vx_true_e)
        ret = vx_true_e;
    else if (retcode == 0 && e->set == vx_false_e)
        ret = vx_false_e;
    else if (retcode == 0 && e->set == vx_true_e)
        ret = vx_true_e;
    return ret;
}
#endif

vx_bool vxWaitEvent(vx_event_t *e, vx_uint32 timeout)
{
    vx_bool ret = vx_false_e;
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    pthread_mutex_lock(&e->mutex);
    if (e->autoreset == vx_false_e) {
        if (e->set == vx_false_e)
            ret = vxWaitEventInternal(e, timeout);
        else
            ret = vx_true_e;
    } else {
        ret = vxWaitEventInternal(e, timeout);
        if (ret == vx_true_e && e->set == vx_true_e)
            e->set = vx_false_e;
        else if (ret == vx_true_e && e->set == vx_false_e)
            ret = vx_false_e;
    }
    pthread_mutex_unlock(&e->mutex);
    return ret;
#elif defined(WIN32) || defined(UNDER_CE)
    DWORD status = WaitForSingleObject(*e, timeout);
    if (status == WAIT_OBJECT_0)
        ret = vx_true_e;
#endif
    return ret;
}

vx_bool vxSetEvent(vx_event_t *e)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    int err = 0;
    pthread_mutex_lock(&e->mutex);
    e->set = vx_true_e;
    err = pthread_cond_broadcast(&e->cond);
    pthread_mutex_unlock(&e->mutex);
    if (err == 0)
        return vx_true_e;
    else
        return vx_false_e;
#elif defined(WIN32) || defined(UNDER_CE)
    return (vx_bool)SetEvent(*e);
#endif
}

vx_bool vxResetEvent(vx_event_t *e)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    pthread_mutex_lock(&e->mutex);
    e->set = vx_false_e;
    pthread_mutex_unlock(&e->mutex);
    return vx_true_e;
#elif defined(WIN32) || defined(UNDER_CE)
    return (vx_bool)ResetEvent(*e);
#endif
}

vx_bool vxJoinThread(vx_thread_t thread, vx_value_t *value)
{
    vx_bool joined = vx_false_e;
    if (thread)
    {
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
        if (pthread_join(thread, (void **)value) == 0)
            joined = vx_true_e;
#elif defined(WIN32) || defined(UNDER_CE)
        DWORD status = WaitForSingleObject(thread, INFINITE);
        if (status == WAIT_OBJECT_0) {
            GetExitCodeThread(thread, (LPDWORD)&value);
            joined = vx_true_e;
        }
#endif
    }
    return joined;
}

void vxSleepThread(vx_uint32 milliseconds)
{
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    struct timespec rtsp;
    rtsp.tv_sec = 0;
    rtsp.tv_nsec = milliseconds * 100000;
    nanosleep(&rtsp, NULL);
#elif defined(WIN32) || defined(UNDER_CE)
    Sleep(milliseconds);
#endif
}

vx_thread_t vxCreateThread(vx_thread_f func, void *arg)
{
    vx_thread_t thread = 0;
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN) || defined(DARWIN)
    pthread_create(&thread, NULL, (pthread_f)func, arg);
#elif defined(WIN32) || defined(UNDER_CE)
    thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, CREATE_SUSPENDED, NULL);
    if (thread)
        ResumeThread(thread);
#endif
    return thread;
}

vx_uint64 vxCaptureTime()
{
    vx_uint64 cap = 0;
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    cap = (vx_uint64)((vx_uint64)t.tv_nsec + (vx_uint64)t.tv_sec*BILLION);
#elif defined(DARWIN)
    cap = mach_absolute_time();
#elif defined(WIN32) || defined(UNDER_CE)
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    cap = (vx_uint64)t.QuadPart;
#endif
    return cap;
}

vx_uint64 vxGetClockRate()
{
    vx_uint64 freq = 0;
#if defined(LINUX) || defined(ANDROID) || defined(__QNX__) || defined(CYGWIN)
    struct timespec t;
    clock_getres(CLOCK_MONOTONIC, &t);
    freq = (vx_uint64)(BILLION/t.tv_nsec);
#elif defined(DARWIN)
    freq = BILLION;
#elif defined(WIN32) || defined(UNDER_CE)
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    freq = (vx_uint64)f.QuadPart;
#endif
    return freq;
}

void vxStartCapture(vx_perf_t *perf)
{
    perf->beg = vxCaptureTime();
}

void vxStopCapture(vx_perf_t *perf)
{
#if defined(DARWIN)
    static mach_timebase_info_data_t    sTimebaseInfo;
#endif
    perf->end = vxCaptureTime();
    perf->tmp = perf->end - perf->beg;
#if defined(DARWIN)
    if (sTimebaseInfo.denom == 0) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    perf->tmp = perf->tmp * sTimebaseInfo.numer / sTimebaseInfo.denom;
#endif
    perf->sum += perf->tmp;
    perf->num++;
    perf->avg = perf->sum / perf->num;
    VX_PRINT(VX_ZONE_PERF, "beg:"VX_FMT_TIME" end:"VX_FMT_TIME" tmp:"VX_FMT_TIME" sum:"VX_FMT_TIME" num:"VX_FMT_TIME" avg:"VX_FMT_TIME"\n",
             perf->beg, perf->end, perf->tmp, perf->sum, perf->num, perf->avg);
}

void vxInitPerf(vx_perf_t *perf)
{
    memset(perf, 0, sizeof(vx_perf_t));
}

void vxPrintQueue(vx_queue_t *q)
{
    vx_uint32 i;
    VX_PRINT(VX_ZONE_OSAL, "Queue: %p, lock=%p s,e=[%d,%d] popped=%s\n",q, &q->lock, q->start_index, q->end_index, (q->popped?"yes":"no"));
    for (i = 0; i < VX_INT_MAX_QUEUE_DEPTH; i++)
    {
        VX_PRINT(VX_ZONE_OSAL, "[%u] = {" VX_FMT_VALUE ", " VX_FMT_VALUE "}\n", i, q->data[i].v1, q->data[i].v2);
    }
}

void vxInitQueue(vx_queue_t *q)
{
    if (q)
    {
        memset(q->data, 0, sizeof(q->data));
        q->start_index = 0;
        q->end_index = -1;
        vxCreateSem(&q->lock, 1);
        vxInitEvent(&q->readEvent, vx_false_e);
        vxInitEvent(&q->writeEvent, vx_false_e);
        vxSetEvent(&q->writeEvent);
    }
}

vx_bool vxWriteQueue(vx_queue_t *q, vx_value_t v1, vx_value_t v2)
{
    vx_bool wrote = vx_false_e;
    if (q)
    {
        // wait for the queue to be writeable
        VX_PRINT(VX_ZONE_OSAL, "About to wait on queue %p\n", q);
        while (vxWaitEvent(&q->writeEvent, VX_INT_FOREVER) == vx_true_e)
        {
            VX_PRINT(VX_ZONE_OSAL, "Signalled!\n");
            vxSemWait(&q->lock);
            if (q->popped == vx_false_e)
            {
                // cause other writers to block
                vxResetEvent(&q->writeEvent);
                // add the value to the data array if space is available
                if (q->start_index != q->end_index)
                {
                    if (q->end_index == -1) // empty
                        q->end_index = q->start_index;
                    q->data[q->end_index].v1 = v1;
                    q->data[q->end_index].v2 = v2;
                    VX_PRINT(VX_ZONE_OSAL, "Wrote %p[%u] = {" VX_FMT_VALUE "," VX_FMT_VALUE "}\n", q, q->end_index, v1, v2);
                    q->end_index = (q->end_index + 1)%VX_INT_MAX_QUEUE_DEPTH;
                    wrote = vx_true_e;

                    // cause other writers to unblock.
                    if (q->start_index != q->end_index)
                        vxSetEvent(&q->writeEvent);

                    vxPrintQueue(q);
                }
                else
                {
                    // two writers may have raced to enter...
                    // the second may have a full queue here...
                }

                if (q->end_index != -1)
                    vxSetEvent(&q->readEvent);
            }
            vxSemPost(&q->lock);
            if (q->popped == vx_true_e || wrote == vx_true_e)
                break;
        }
    }
    return wrote;
}

vx_bool vxReadQueue(vx_queue_t *q, vx_value_t *pv1, vx_value_t *pv2)
{
    vx_bool red = vx_false_e;
    if (q)
    {
        VX_PRINT(VX_ZONE_OSAL, "About to wait on queue %p\n", q);
        while (vxWaitEvent(&q->readEvent, VX_INT_FOREVER) == vx_true_e)
        {
            VX_PRINT(VX_ZONE_OSAL, "Signalled!\n");
            vxSemWait(&q->lock);
            if (q->popped == vx_false_e)
            {
                if (q->end_index != -1) // not empty
                {
                    vx_value_t v1, v2;
                    v1 = q->data[q->start_index].v1;
                    v2 = q->data[q->start_index].v2;
                    VX_PRINT(VX_ZONE_OSAL, "Read %p[%u] = {" VX_FMT_VALUE "," VX_FMT_VALUE "}\n",q,q->start_index, v1, v2);
                    q->data[q->start_index].v1 = q->data[q->start_index].v2 = 0;
                    q->start_index = (q->start_index + 1)%VX_INT_MAX_QUEUE_DEPTH;
                    red = vx_true_e;
                    if (q->start_index == q->end_index) // wrapped to empty
                    {
                        vxResetEvent(&q->readEvent);
                        q->end_index = -1;
                    }
                    vxPrintQueue(q);
                    if (pv1)
                        *(vx_value_t *)pv1 = v1;
                    if (pv2)
                        *(vx_value_t *)pv2 = v2;
                }

                vxSetEvent(&q->writeEvent);
            }
            vxSemPost(&q->lock);
            if (q->popped == vx_true_e || red == vx_true_e)
                break;
        }
        VX_PRINT(VX_ZONE_OSAL, "Leaving with %d\n", red);
    }
    return red;
}

void vxPopQueue(vx_queue_t *q)
{
    if (q)
    {
        vxSemWait(&q->lock);
        q->popped = vx_true_e;
        vxSetEvent(&q->readEvent);
        vxSetEvent(&q->writeEvent);
        vxSemPost(&q->lock);
    }
}

void vxDeinitQueue(vx_queue_t *q)
{
    if (q)
    {
        q->start_index = 0;
        q->end_index = -1;
        vxDestroySem(&q->lock);
        vxDeinitEvent(&q->readEvent);
        vxDeinitEvent(&q->writeEvent);
    }
}

vx_module_handle_t vxLoadModule(vx_char * name)
{
    vx_module_handle_t mod;

#if defined(LINUX) || defined(ANDROID) || defined(DARWIN) || defined(__QNX__) || defined(CYGWIN)
    mod = dlopen(name, RTLD_NOW|RTLD_LOCAL);
    if (mod == 0)
    {
        VX_PRINT(VX_ZONE_ERROR, "%s\n", dlerror());
    }
#elif defined(WIN32)
    mod = LoadLibrary(name);
#endif
    return mod;
}

void vxUnloadModule(vx_module_handle_t mod)
{
#if defined(LINUX) || defined(ANDROID) || defined(DARWIN) || defined(__QNX__) || defined(CYGWIN)
    dlclose(mod);
#elif defined(WIN32)
    CloseHandle(mod);
#endif
}

vx_symbol_t vxGetSymbol(vx_module_handle_t mod, vx_char *name)
{
#if defined(LINUX) || defined(ANDROID) || defined(DARWIN) || defined(__QNX__) || defined(CYGWIN)
    return (vx_symbol_t)dlsym(mod, name);
#elif defined(WIN32)
    return (vx_symbol_t)GetProcAddress(mod, name);
#endif
}

/******************************************************************************/
// EXTERNAL API (NO COMMENTS HERE, SEE HEADER FILES)
/******************************************************************************/

