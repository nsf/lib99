CFLAGS:=$(shell pkg-config --cflags check)
LIBS:=$(shell pkg-config --libs check) -lm
CC:=clang
FILES:=test_main.c test_suites.h\
	strstr.c strstr.h strstr_test.c\
	linear_math.c linear_math.h linear_math_test.c

SOURCES:=$(filter-out %.h,$(FILES))
	

test: test_main
	@./test_main

test_main: $(FILES)
	clang -std=c99 -Wall -pedantic -O0 -g $(CFLAGS) $(LIBS) -o test_main $(SOURCES)

clean:
	rm -rf test_main
