#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


union semun {
    int val;            
    struct semid_ds *buf;   
    ushort *array;         
    struct seminfo *__buf;  
    void *__pad;
    };

void init_semaphore(int semid,int semnum,int semval);

int sem_wait(int semid,int semnum);

int sem_signal(int semid,int semnum);

#endif
