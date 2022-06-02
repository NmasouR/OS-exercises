#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include<unistd.h>
#include <errno.h>

#include "semaphores.h"


#define SIZE 50

int main(int argc,char *argv[])
{
    int sem_id;
    int scan;
    int op_error;
    char rensponse[5];
    char message[SIZE];
    int shmid;
    char *shm_mem;
    char *random = argv[1];
    char *finalmsg;

    key_t key = ftok("./P1", 'A');


    if((sem_id = semget(key,8, IPC_CREAT | IPC_EXCL | 0666)) == -1)//semaphores already exists
    {
        if((sem_id = semget(key,8,0666)) == -1)
        {
            perror("error cant acess semaphores");
            exit(1);
        }        
    }
    else //init semaphores
    {
        for(int i = 1;i<7;i++)
        {
            init_semaphore(sem_id,i,1);
        }
        init_semaphore(sem_id,0,0);
        init_semaphore(sem_id,7,0);
    }

    if((shmid = shmget(key,4*SIZE,IPC_CREAT | 0666)) == -1)
    {
        perror("error in shared mem creation");
        exit(2);
    }

    if((shm_mem = (char *)shmat(shmid,NULL,0666)) == (char *) -1)
    {
        perror("error attaching the shared mem");
        exit(3);
    }
    if(argc == 2)
     {
        if((sem_wait(sem_id,7)) == -1)
        {
            perror("error waiting the semaphores");
            exit(4);
        }
        strncpy(message,shm_mem + 3*SIZE,SIZE);
        finalmsg = message;
        finalmsg[strlen(message) - 16] = '\0';
        printf("P1:%s",finalmsg);
        printf("Please write your message:");
        fgets(message,30,stdin);
        strncpy(shm_mem + 3*SIZE, message, SIZE);
        sem_signal(sem_id,6);
        execlp("./ENC2",argv[1],"P2",(char *) 0);
     }
    else if(argc == 3)
    {
        if((sem_wait(sem_id,7)) == -1)
        {
            perror("error waiting the semaphores");
            exit(5);
        }
        strncpy(message,shm_mem + 3*SIZE,SIZE);
        finalmsg = message;
        finalmsg[strlen(message) - 16] = '\0';
        printf("P1:%s",finalmsg);
        if(strcmp(message,"TERM\n") == 0)
        {
            semctl(sem_id, 7, IPC_RMID);
            semctl(sem_id, 6, IPC_RMID);
            semctl(sem_id, 5, IPC_RMID);
            semctl(sem_id, 4, IPC_RMID);
            semctl(sem_id, 3, IPC_RMID);
            semctl(sem_id, 2, IPC_RMID);
            semctl(sem_id, 1, IPC_RMID);
            semctl(sem_id, 0, IPC_RMID);            
            shmdt(shm_mem);
            shmctl(shmid, IPC_RMID, NULL);
            return 0;
        }
        printf("Please write your message:");
        fgets(message,30,stdin);
        strncpy(shm_mem + 3*SIZE, message, SIZE);
        sem_signal(sem_id,6);
        execlp("./ENC2",argv[0],"P2",(char *) 0);
    }
}