EXES := test_cursor.out test_grammar.out test_lexer.out \
	test_load_grammar.out test_node.out test_parser_2.out \
	test_parser_3.out test_parser.out

CC := g++ -g -O3 -std=c++20
CFLAGS := -pedantic -Wall
LFLAGS := -pedantic -Wall

TARGETS := cursor grammar lexer node parser load_grammar
OBJS := $(TARGETS:%=../build/%.o)
HEADS := $(TARGETS:%=../src/%.hpp)

.PHONY:	build
build:	$(EXES)

%.out:	%.o $(OBJS)
	$(CC) $(LFLAGS) -o $@ $^
	./$@

%.o:	%.cpp $(HEADS) Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS):	$(HEADS) $(TARGETS:%=../src/%.cpp)
	$(MAKE) -C ..

.PHONY:	clean
clean:
	rm -f *.dot *.out *.png *.o
