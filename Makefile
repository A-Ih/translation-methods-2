srcs = \
	   lexer.hh

format: FORCE
	clang-format -i --style=Google *.hh *.cc

# %: %.cpp FORCE
# 	clang++ -g -O2 -std=c++20 -static $< -o $@

FORCE:

