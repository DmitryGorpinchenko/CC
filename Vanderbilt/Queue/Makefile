CC		= g++
CFILES	= LQueue-test.cpp LQueue.cpp AQueue.cpp Array.cpp
HFILES	= LQueue.h AQueue.h Array.h
LOFILES	= LQueue-test.o LQueue.o
AOFILES	= AQueue-test.o AQueue.o Array.o

DFLAGS	= -g
CFLAGS  = -std=c++14 $(DFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

all: LQueue-test AQueue-test

LQueue-test: $(LOFILES)
	$(CC) $(LOFILES) -o $@

AQueue-test: $(AOFILES)
	$(CC) $(AOFILES) -o $@

clean:
	rm -f *.o *.out *~ core

realclean: clean
	rm -rf LQueue-test AQueue-test

Array.o: Array.cpp Array.h Array.inl
LQueue.o: LQueue.cpp LQueue.h
AQueue.o: AQueue.cpp AQueue.h
LQueue-test.o: LQueue-test.cpp LQueue.h LQueue.cpp
AQueue-test.o: AQueue-test.cpp AQueue.h AQueue.cpp Array.cpp Array.h

