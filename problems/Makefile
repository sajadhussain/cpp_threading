.PHONY: all
all: ./phil ./prod_cons ./smokers ./savages

./savages: ./savages.cpp ../sem/sem.o ../include/sem.h

./smokers: ./smokers.cpp ../sem/sem.o ../include/sem.h

./phil: CXXFLAGS+=-DSOLN_RIGHTIE
./phil: ./phil.cpp ../sem/sem.o ../include/sem.h

./prod_cons: ./prod_cons.cpp ../include/queue.h

.PHONY: clean
clean:
	rm -f ./phil ./prod_cons ./smokers ./savages
