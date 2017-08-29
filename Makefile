CXX=clang++ -std=c++14 -g -O3
CC=clang
OBJS=lang.o

parens: $(OBJS)

plus: $(OBJS)

pal: $(OBJS)

.PHONY: clean

clean:
	rm $(OBJS)
