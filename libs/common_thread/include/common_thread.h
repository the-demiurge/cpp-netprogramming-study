#ifndef NETWORK_PROGRAMMING_COMMON_THREAD_H
#define NETWORK_PROGRAMMING_COMMON_THREAD_H

#ifdef _WIN32

#include<windows.h>

typedef HANDLE THREAD_HANDLE;
typedef DWORD PROCESS_ID;

#define THREAD_RESULT DWORD

#elif __linux__


#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

typedef int THREAD_HANDLE;
typedef pid_t PROCESS_ID;

#define THREAD_RESULT void*

#endif

typedef THREAD_RESULT (*thread_function)(void*);
typedef LPVOID THREAD_PARAM;

THREAD_HANDLE create_thread(thread_function, THREAD_PARAM);

PROCESS_ID get_process_id();

void wait_thread(THREAD_HANDLE);

void wait_thread(THREAD_HANDLE*, int size);

#endif //NETWORK_PROGRAMMING_COMMON_THREAD_H
