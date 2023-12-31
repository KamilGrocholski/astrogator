bin_dir = bin

source_files_main = src/main.c
source_files = src/compiler/lexer.c src/compiler/parser.c src/compiler/program.c
output_binary = $(bin_dir)/astrogator

source_test_files_main = test/main.test.c
source_test_files = test/lexer/lexer.test.c test/parser/parser.test.c
output_test_binary = $(bin_dir)/test

build:
	mkdir -p $(bin_dir)
	gcc $(source_files_main) $(source_files) -o ./$(output_binary)
	./$(output_binary) -h

run_tests: 
	mkdir -p $(bin_dir)
	gcc $(source_test_files_main) $(source_files) $(source_test_files) -o ./$(output_test_binary)
	./$(output_test_binary) -h

clean: 
	rm -rf $(bin_dir)
