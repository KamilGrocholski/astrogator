source_files = src/main.c src/compiler/lexer/lexer.c
output_binary = bin/astrogator

source_tests_files = tests/lexer.c
output_tests_binary = bin/tests

build:
	rm -rf bin && mkdir bin || echo
	gcc $(source_files) -o $(output_binary)
	./bin/astrogator -h

test:
	rm -rf bin && mkdir bin || echo
	gcc $(source_tests_files) -o $(output_tests_binary)
	./bin/tests -h
