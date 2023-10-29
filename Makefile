source_files_main = src/main.c
source_files = src/compiler/lexer/lexer.c
output_binary = bin/astrogator

source_test_files_main = test/main.test.c
source_test_files = test/lexer/lexer.test.c
output_test_binary = bin/test

build:
	gcc $(source_files_main) $(source_files) -o $(output_binary)
	./bin/astrogator -h

run_tests: 
	gcc $(source_test_files_main) $(source_files) $(source_test_files) -o $(output_test_binary)
	./bin/test -h

clean: 
	rm -rf bin
