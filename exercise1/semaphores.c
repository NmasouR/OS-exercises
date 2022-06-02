#include <stdio.h>
#include "semaphores.h"


void init_semaphore(int semid,int semnum,int semval)
{
    union semun semopts;
    semopts.val = semval;
    semctl(semid,semnum,SETVAL,semopts);
}

int sem_wait(int semid,int semnum)
{
    int error;

     struct sembuf op_lock[2] = {
        semnum,0,0,
        semnum,1,0
    };

    if((error = semop(semid,op_lock,2)) == -1)
    {
        printf("error upon waiting");
        return -1;
    } 
    return 0;  
}

int sem_signal(int semid,int semnum)
{
    int error;
    struct sembuf op_unlock[1] = {
        semnum,-1,IPC_NOWAIT // if sem == 0 fails 
    };
    if((error = semop(semid,op_unlock,1)) == -1)
    {
        printf("error upon waiting");
        return -1;
    } 
    return 0;  

}