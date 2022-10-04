srcs = \
	   defs.hh \
	   lexer.hh \
	   tree.hh \
	   parser.hh \
	   parser.cc \
	   main.cc


CC=clang++
# clang 13 doesn't support <format>
CFLAGS= \
		-Wall \
		-static \
		-O2 \
		-g \
		-std=c++20

all: FORCE
	$(CC) $(CFLAGS) main.cc parser.cc -o parser

format: FORCE
	clang-format -i --style=file *.hh *.cc

FORCE:

