CXXFLAGS += -Wall -std=c++98 -pedantic -O3

LU_OBJS = matrix.o vector_utils.o matrix_utils.o

OBJS = io_utils.o vector_utils.o matrix.o

default: jacobis lu lu-test jacobis-test

all: default

jacobis: $(OBJS) jacobis.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lu: $(LU_OBJS) lu.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lu-test: $(LU_OBJS) lu-test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

jacobis-test: $(OBJS) jacobis-test.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f jacobis.o jacobis
	rm -f matrix.o lu.o matrix_utils.o lu
	rm -f lu-test.o lu-test jacobis-test.o jacobis-test

