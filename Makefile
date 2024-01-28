bin_dir = bin
flags = -DDEBUG -pedantic -W -g

source_files_main = src/main.c
source_files = src/lexer.c src/parser.c src/node.c src/object.c src/code.c src/vm.c src/compiler.c src/st.c
output_binary = $(bin_dir)/astrogator

source_test_files_main = test/main.test.c
source_test_files = 
output_test_binary = $(bin_dir)/test

build:
	mkdir -p $(bin_dir)
	gcc $(flags) $(source_files_main) $(source_files) -o ./$(output_binary)
	./$(output_binary) tocomp.ag -h 

run_tests: 
	mkdir -p $(bin_dir)
	gcc $(flags) $(source_test_files_main) $(source_files) $(source_test_files) -o ./$(output_test_binary)
	./$(output_test_binary) tocomp.ag -h
clean: 
	rm -rf $(bin_dir)
