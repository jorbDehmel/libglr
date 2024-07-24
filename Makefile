TARGETS := main.out

CC := g++ -g -O3 -std=c++20
CFLAGS := -pedantic -Wall
LFLAGS := -pedantic -Wall
OBJS := build/cursor.o build/grammar.o build/node.o \
	build/parser.o build/lexer.o build/load_grammar.o
HEADS := src/cursor.hpp src/grammar.hpp src/node.hpp \
	src/parser.hpp src/token.hpp src/lexer.hpp \
	src/load_grammar.hpp

.PHONY:	all
all:	$(TARGETS)

%.out:	build/%.o $(OBJS) $(HEADS) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJS) $<

test/%.out:	test/%.o $(OBJS) $(HEADS) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJS) $<

build/%.o:	src/%.cpp $(HEADS) Makefile | format build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir build

.PHONY:	format
format:
	find . -type f 											   \
		\( -iname '*.cpp' -or -iname '*.hpp' \)				   \
		-exec clang-format -i "{}" \;

.PHONY:	clean
clean:
	rm -rf build $(TARGETS)
