#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include<unistd.h>
#include <errno.h>
#include <time.h> 


#include "semaphores.h"

#define SIZE 50


int main(int argc,char *argv[])
{
    int random = atoi(argv[0]); 
    int sem_id, error,shmid;
    char * shm_mem;
    char string[SIZE];
    int j;
    time_t t;
    srand((unsigned) time(&t));

    key_t key = ftok("./P1", 'A');

    if((sem_id = semget(key,8,0666)) == -1)
    {
        perror("error cant acess semaphores");
        exit(1);
    }

    if((shmid = shmget(key,4*SIZE,0666)) == -1)
    {
        perror("error cant acess mem");
        exit(2);
    }

    if((shm_mem = (char *)shmat(shmid,NULL,0666)) == (char *) -1)
    {
        perror("error attaching the shared mem");
        exit(3);
    }

    if(strcmp(argv[1],"ENC1") == 0 || strcmp(argv[1],"ENC2B") == 0) //called from ENC1 or request for retransmission from ENC2
    {
        sem_signal(sem_id,4);
        if((error = sem_wait(sem_id,3)) == -1)
        {
            perror("error waiting the semaphores");
            exit(4);
        }
        else
        {
            strncpy(string, shm_mem + SIZE, SIZE);

            for(int i = 0;i<strlen(string) - 16 - 1;i++) //16 bytes for checksum 1 byte for /0 of the init string
            {
                if((j = rand()%100) < random) //random noise
                {
                    string[i] = 'X';
                }
            }
            if((error = sem_wait(sem_id,4)) == -1)
            {
            perror("error waiting the semaphores");
            exit(5);
            }
            else
            {
                strncpy(shm_mem + 2*SIZE, string, SIZE);
            }
            sem_signal(sem_id,5);
            execlp("./ENC2",argv[0],"CHAN",NULL);
        }
        
    }
    else if(strcmp(argv[1],"ENC2") == 0 || strcmp(argv[1],"ENC1B") == 0) //called from ENC2 or request for retransmission from ENC1
    {
        sem_signal(sem_id,3);
        

        if((error = sem_wait(sem_id,4)) == -1)
        {
            perror("error waiting the semaphores");
            exit(6);        
        }
        else
        {
            strncpy(string, shm_mem + 2*SIZE, SIZE);
            for(int i = 0;i<strlen(string) -16 -1;i++) //16 bytes for checksum 1 byte for /0 of the init string
            {
                if((j = rand()%100) < random)
                {
                    string[i] = 'X';
                }
            }
            if((error = sem_wait(sem_id,3)) == -1)
            {
                perror("error waiting the semaphores");
                exit(7);
            }
            else
            {
                strncpy(shm_mem + SIZE, string, SIZE);
                sem_signal(sem_id,2);
                execlp("./ENC1",argv[0],"CHAN",NULL);
            }
        }
    }

}