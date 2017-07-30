CXX    = g++
CFILES = main.cpp Array.cpp
OFILES = main.o Array.o
DFLAGS = -g
SFLAGS = -std=c++11
CFLAGS = $(DFLAGS) $(SFLAGS) # -DUSE_TEMPLATES -D__INLINE__ -DUSE_EXCEPTIONS

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

main: $(OFILES)
	$(CXX) $(CFLAGS) -o $@ $(OFILES) $(LIBS)

clean:
	rm -f *.o *.out *~ core

realclean: clean
	rm -fr main Templates.DB

main.o: main.cpp Array.h Array.inl Array.cpp
Array.o: Array.h Array.inl Array.cpp

