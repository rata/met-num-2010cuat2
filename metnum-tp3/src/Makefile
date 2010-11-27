CXXFLAGS += -Wall -std=c++98 -pedantic -O3 -g

# Quick&dirty
matrix.o: matrix.hpp matrix.cc
io_utils.o: io_utils.hpp io_utils.cc
vector_utils.o: vector_utils.hpp vector_utils.cc

OBJS = matrix.o io_utils.o vector_utils.o convenciones.o

default: givens

all: default

givens: $(OBJS) givens.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f matrix.o givens.o givens
