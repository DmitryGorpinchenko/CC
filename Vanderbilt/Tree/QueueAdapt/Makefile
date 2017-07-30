MAKEFILE = Makefile
CC		 = g++ -std=c++14
TFILES   = LQueue.o STLQueue.o AQueue.o Array.o Queue.o adapter-test.o
LDFLAGS	 = -g
CFLAGS	 = -ggdb

.cpp.o:
	$(CC) $(CFLAGS) -c $<

all: adapter-test

adapter-test: $(TFILES)
	$(CC) $(LDFLAGS) $(TFILES) -o $@

clean:
	rm -f *.o *.out *~ core

realclean: clean
	rm -rf adapter-test

Array.o: Array.cpp Array.h Array.inl
LQueue.o: LQueue.cpp LQueue.h
STLQueue.o: STLQueue.h
AQueue.o: AQueue.cpp AQueue.h
Queue.o: Queue.cpp Queue.h
adapter-test.o: adapter-test.cpp Queue.h Queue.cpp AQueue.h AQueue.cpp LQueue.h LQueue.cpp STLQueue.cpp

