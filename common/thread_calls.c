/**
 * xrdp: A Remote Desktop Protocol server.
 *
 * Copyright (C) Jay Sorg 2004-2012
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * thread calls
 */

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#endif

#include "thread_calls.h"
#include "os_calls.h"

/* returns error */
#if defined(_WIN32)
int tc_thread_create(unsigned long (__stdcall *start_routine)(void *), void *arg)
{
	int rv = 0;
	DWORD thread_id = 0;
	HANDLE thread = (HANDLE)0;

	/* CreateThread returns handle or zero on error */
	thread = CreateThread(0, 0, start_routine, arg, 0, &thread_id);
	rv = !thread;
	CloseHandle(thread);
	return rv;
}
#else
int tc_thread_create(void * (* start_routine)(void *), void *arg)
{
	int rv = 0;
	pthread_t thread = (pthread_t) 0;

	g_memset(&thread, 0x00, sizeof(pthread_t));

	/* pthread_create returns error */
	rv = pthread_create(&thread, 0, start_routine, arg);

	if (!rv)
	{
		rv = pthread_detach(thread);
	}

	return rv;
}
#endif

LONG_PTR tc_get_threadid(void)
{
#if defined(_WIN32)
	return (LONG_PTR)GetCurrentThreadId();
#else
	return (LONG_PTR) pthread_self();
#endif
}

/* returns boolean */
int tc_threadid_equal(LONG_PTR tid1, LONG_PTR tid2)
{
#if defined(_WIN32)
	return tid1 == tid2;
#else
	return pthread_equal((pthread_t) tid1, (pthread_t) tid2);
#endif
}

LONG_PTR tc_mutex_create(void)
{
#if defined(_WIN32)
	return (LONG_PTR)CreateMutex(0, 0, 0);
#else
	pthread_mutex_t *lmutex;

	lmutex = (pthread_mutex_t *) g_malloc(sizeof(pthread_mutex_t), 0);
	pthread_mutex_init(lmutex, 0);
	return (LONG_PTR) lmutex;
#endif
}

void tc_mutex_delete(LONG_PTR mutex)
{
#if defined(_WIN32)
	CloseHandle((HANDLE)mutex);
#else
	pthread_mutex_t *lmutex;

	lmutex = (pthread_mutex_t *) mutex;
	pthread_mutex_destroy(lmutex);
	free(lmutex);
#endif
}

int tc_mutex_lock(LONG_PTR mutex)
{
#if defined(_WIN32)
	WaitForSingleObject((HANDLE)mutex, INFINITE);
	return 0;
#else
	pthread_mutex_lock((pthread_mutex_t *) mutex);
	return 0;
#endif
}

int tc_mutex_unlock(LONG_PTR mutex)
{
	int rv = 0;
#if defined(_WIN32)
	ReleaseMutex((HANDLE)mutex);
#else

	if (mutex != 0)
	{
		rv = pthread_mutex_unlock((pthread_mutex_t *) mutex);
	}

#endif
	return rv;
}

LONG_PTR tc_sem_create(int init_count)
{
#if defined(_WIN32)
	HANDLE sem;

	sem = CreateSemaphore(0, init_count, init_count + 10, 0);
	return (LONG_PTR)sem;
#else
	sem_t *sem = (sem_t *) NULL;

	sem = (sem_t *) g_malloc(sizeof(sem_t), 0);
	sem_init(sem, 0, init_count);
	return (LONG_PTR) sem;
#endif
}

void tc_sem_delete(LONG_PTR sem)
{
#if defined(_WIN32)
	CloseHandle((HANDLE)sem);
#else
	sem_t *lsem;

	lsem = (sem_t *) sem;
	sem_destroy(lsem);
	free(lsem);
#endif
}

int tc_sem_dec(LONG_PTR sem)
{
#if defined(_WIN32)
	WaitForSingleObject((HANDLE)sem, INFINITE);
	return 0;
#else
	sem_wait((sem_t *) sem);
	return 0;
#endif
}

int tc_sem_inc(LONG_PTR sem)
{
#if defined(_WIN32)
	ReleaseSemaphore((HANDLE)sem, 1, 0);
	return 0;
#else
	sem_post((sem_t *) sem);
	return 0;
#endif
}
