SRCS = \
	   defs.hh \
	   lexer.hh \
	   lexer.cc \
	   tree.hh \
	   parser.hh \
	   parser.cc \
	   main.cc


CC=clang++
CFLAGS= \
		-Wall \
		-fsanitize=address,leak,undefined \
		-O2 \
		-g \
		-std=c++20

all: format parser test

parser:
	$(CC) $(CFLAGS) main.cc lexer.cc parser.cc -o parser

test: parser
	./test.sh

format: FORCE
	clang-format -i --style=file $(SRCS)

picture: parser
	./parser >pic.dot && \
	dot -T svg pic.dot >pic.svg && \
	open pic.svg

clean: FORCE
	rm -f parser pic.dot pic.svg

FORCE:

