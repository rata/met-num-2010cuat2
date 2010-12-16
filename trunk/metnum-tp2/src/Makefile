CXXFLAGS += -Wall -std=c++98 -pedantic -O3

LU_OBJS = matrix.o matrix_utils.o

OBJS = io_utils.o matrix.o matrix_utils.o

default: jacobis lu

all: default jacobis-test lu-test

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

