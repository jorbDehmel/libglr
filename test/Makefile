TARGETS := cursor grammar lexer node parser load_grammar
CC := g++ -g -O3 -std=c++20
CFLAGS := -pedantic -Wall
LFLAGS := -pedantic -Wall

EXES := $(TARGETS:%=test_%.out)
OBJS := $(TARGETS:%=../build/%.o)
HEADS := $(TARGETS:%=../src/%.hpp)

.PHONY:	build
build:	$(EXES)

.PHONY:	run
run:	build
	for test in $(EXES) ; do                                   \
		./$$test ;                                             \
	done

%.out:	%.o $(OBJS)
	$(CC) $(LFLAGS) -o $@ $^

%.o:	%.cpp $(HEADS) Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS):	$(HEADS) $(TARGETS:%=../src/%.cpp)
	$(MAKE) -C ..
