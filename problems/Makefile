CXXFLAGS+=-I../include/

./phil: CXXFLAGS+=-DSOLN_RIGHTIE
./phil: ./phil.cpp ../sem/sem.o

.PHONY: clean
clean:
	rm -f ./phil
