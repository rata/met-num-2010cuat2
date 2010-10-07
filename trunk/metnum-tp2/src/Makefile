CXXFLAGS += -Wall -std=c++98 -pedantic -o3

LU_OBJS = matrix.o

OBJS = io_utils.o

default: jacobis lu

all: default

jacobis: $(OBJS) jacobis.o
	$(CXX) $(CXXFLAGS) $^ -o $@

lu: $(LU_OBJS) lu.o
	$(CXX) $(CXXFLAGS) $^ -o $@


clean:
	rm -f $(OBJS)
	rm -f jacobis.o jacobis
	rm -f matrix.o lu.o lu

