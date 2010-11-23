CXXFLAGS += -Wall -std=c++98 -pedantic -O3 -Weffc++

OBJS = matrix.o io_utils.o vector_utils.o

default: givens

all: default

givens: $(OBJS) givens.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f matrix.o givens.o givens
