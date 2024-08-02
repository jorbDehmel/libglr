TARGETS := main.out libglr.so

CC := g++ -g -O3 -std=c++20
CFLAGS := -pedantic -Wall
LFLAGS := -pedantic -Wall
OBJS := build/cursor.o build/grammar.o build/node.o \
	build/parser.o build/lexer.o build/load_grammar.o
HEADS := src/cursor.hpp src/grammar.hpp src/node.hpp \
	src/parser.hpp src/token.hpp src/lexer.hpp \
	src/load_grammar.hpp
SOFLAGS := -fPIC -shared

.PHONY:	all
all:	$(TARGETS)

.PHONY:	install
install:	$(TARGETS)
	cp libglr.so /usr/lib
	mkdir -p /usr/include/glr
	cp src/*.hpp /usr/include/glr

.PHONY:	uninstall
uninstall:
	rm -f /usr/lib/libglr.so
	rm -rf /usr/include/glr

%.out:	build/%.o $(OBJS) $(HEADS) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJS) $<

%.so:	$(OBJS:%.o=%.o.fpic) $(HEADS) Makefile
	$(CC) $(SOFLAGS) -o $@ $(OBJS:%.o=%.o.fpic)

test/%.out:	test/%.o $(OBJS) $(HEADS) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJS) $<

build/%.o:	src/%.cpp $(HEADS) Makefile | format build
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.o.fpic:	src/%.cpp $(HEADS) Makefile | format build
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

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
