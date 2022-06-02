askisi: askisi.o LRU.o LRU.h clock.o clock.h hash.o hash.h
	g++ -o askisi askisi.o LRU.o clock.o hash.o

askisi.o: askisi.cpp
	g++ -std=c++11 -c askisi.cpp

LRU.o: LRU.cpp LRU.h
	g++ -c LRU.cpp

clock.o: clock.cpp clock.h
	g++ -c clock.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

clean:
	rm *.o all