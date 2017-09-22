CXX=clang++ -std=c++14 -g -O1
CC=clang
OBJS= simple.o compound.o node_t.o

.PHONY: all

all: parens plus pal

parens: $(OBJS)

plus: $(OBJS)

pal: $(OBJS)

.PHONY: clean

clean:
	rm parens plus pal $(OBJS)
