all: P1 P2 ENC1 ENC2 CHAN

P1: P1.o semaphores.o semaphores.h
	gcc -o P1 P1.o semaphores.o

P1.o: P1.c
	gcc -c P1.c

semaphores.o: semaphores.c semaphores.h
	gcc -c  semaphores.c

P2: P2.o semaphores.o semaphores.h
	gcc -o P2 P2.o semaphores.o

P2.o: P2.c
	gcc -c P2.c

ENC1: ENC1.o semaphores.o semaphores.h
	gcc -o ENC1 ENC1.o semaphores.o -lcrypto

ENC1.o: ENC1.c
	gcc -c ENC1.c

ENC2: ENC2.o semaphores.o semaphores.h
	gcc -o ENC2 ENC2.o semaphores.o -lcrypto

ENC2.o: ENC2.c
	gcc -c ENC2.c

CHAN: CHAN.o semaphores.o semaphores.h
	gcc -o CHAN CHAN.o semaphores.o

CHAN.o: CHAN.c 
	gcc -c CHAN.c

clean:
	rm *.o all