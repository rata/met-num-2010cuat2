CXXFLAGS += -Wall -std=c++98 -pedantic -g -Weffc++

default: art-attack

# Quick&dirty

matrix.o: matrix.hpp matrix.cc matrix_utils.hpp
matrix_utils.o: matrix_utils.cc matrix_utils.hpp
io_utils.o: io_utils.hpp io_utils.cc
vector_utils.o: vector_utils.hpp vector_utils.cc


OBJS = matrix.o matrix_utils.o io_utils.o vector_utils.o convenciones.o

all: default

art-attack: $(OBJS) art-attack.o
	$(CXX) $(CXXFLAGS) $^ -o $@

gen-data: $(OBJS) gen-data.o
	$(CXX) $(CXXFLAGS) $^ -o $@
test: $(OBJS) test.o
	$(CXX) $(CXXFLAGS) $^ -o $@


clean:
	rm -f $(OBJS)
	rm -f art-attack.o gen-data.o art-attack gen-data
