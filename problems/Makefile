.PHONY: all
all: ./phil ./prod_cons

./phil: CXXFLAGS+=-DSOLN_RIGHTIE
./phil: ./phil.cpp ../sem/sem.o ../include/sem.h

./prod_cons: ./prod_cons.cpp ../include/queue.h

.PHONY: clean
clean:
	rm -f ./phil ./prod_cons
