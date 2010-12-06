CXXFLAGS += -std=c++98 -pedantic -O3 -Weffc++

default: art-attack gen-data

# Quick&dirty
matrix.o: matrix.hpp matrix.cc matrix_utils.hpp
matrix_utils.o: matrix_utils.cc matrix_utils.hpp

OBJS = matrix.o matrix_utils.o convenciones.o

all: art-attack gen-data test

art-attack: $(OBJS) art-attack.o
	$(CXX) $(CXXFLAGS) $^ -o $@

gen-data: $(OBJS) gen-data.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(OBJS) test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f art-attack.o art-attack
	rm -f gen-data.o gen-data
	rm -f test.o test
