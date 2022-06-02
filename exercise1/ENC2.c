#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <openssl/md5.h>
#include<unistd.h>
#include <errno.h>


#include "semaphores.h"

#define SIZE 50

//Works like ENC1!!

int main(int argc,char *argv[])
{
    int random = atoi(argv[0]);
    unsigned char digest[17];
    int sem_id, error,shmid;
    char * shm_mem;
    char string[SIZE];
    char msg[SIZE];
    char  *checksum;
    int comp = 0;

    MD5_CTX context;
    MD5_Init(&context);


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

    if(strcmp(argv[1],"P2") == 0)
    {
        sem_signal(sem_id,5);
        if((error = sem_wait(sem_id,6)) == -1)
        {
            perror("error waiting the semaphores");
            exit(4);
        }
        else
        {
            strncpy(string, shm_mem + 3*SIZE, SIZE);
            MD5_Update(&context, string, strlen(string));
            MD5_Final(digest, &context);
            digest[16] = '\0';
            strcat(string,digest);
            if((error = sem_wait(sem_id,5)) == -1)
            {
                perror("error waiting the semaphores");
                exit(5);
            }
            strncpy(shm_mem + 2*SIZE, string, SIZE);
            sem_signal(sem_id,4);
            execlp("./CHAN",argv[0],"ENC2",NULL);
        }
    }
    else if(strcmp(argv[1],"CHAN") == 0)
    {
        if((error = sem_wait(sem_id,5)) == -1)
        {
            perror("error waiting the semaphores");
            exit(6);
        }
        strncpy(string,shm_mem + 2*SIZE,SIZE);
        strncpy(msg,string,SIZE);
        checksum = &string[strlen(string)  - 16];
        msg[strlen(msg) - 16] = '\0';   
        MD5_Update(&context, msg, strlen(msg));
        MD5_Final(digest, &context);
        digest[16] = '\0';
        if(strcmp(digest,checksum) != 0)
        {
            comp = 1;
        }
        if(comp == 0) // send to P2
        {
            sem_signal(sem_id,6);
            if((error = sem_wait(sem_id,6)) == -1)
            {
                printf("error waiting sem 6\n");
                exit(7);
            }
            strncpy(shm_mem + 3*SIZE, string, SIZE);
            sem_signal(sem_id,7);
            if(strstr(string, "TERM") != NULL)
            {
                return 0;
            }
            execlp("./P1",argv[0],"ENC2","EXTRA_PAR",NULL);
        }
        else if(comp == 1)
        {
             sem_signal(sem_id,3);
             execlp("./CHAN",argv[0],"ENC2B",NULL);
        }
    }

}