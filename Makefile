CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++17 -pthread -g -Og
LDFLAGS=

export CXX
export CXXFLAGS
export LDFLAGS

.PHONY: all
all:
	cd ./sem/ && $(MAKE)
	cd ./problems/ && $(MAKE)

.PHONY: clean
clean:
	cd ./sem/ && $(MAKE) clean
	cd ./problems/ && $(MAKE) clean
