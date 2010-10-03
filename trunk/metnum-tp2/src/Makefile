CXXFLAGS += -Wall -std=c++98 -pedantic -Wno-long-long

OBJS = io_utils.o

default: jacobis

all: default

jacobis: $(OBJS) jacobis.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)
	rm -f jacobis.o jacobis

