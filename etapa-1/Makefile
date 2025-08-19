
main: main.o
	@printf "# Linking main object\n"
	gcc out/*.o -o out/main
	@printf "# Finished: Linking main object\n\n"

main.o: 
	@printf "# Compiling main object\n"
	gcc -c src/*.c 
	@printf "# Finished: Compiling main object\n\n"

test: test.o	
	g++ out/test.o -o out/test -lgtest -lgtest_main

test.o: 
	g++ -c tests/test.cpp -o out/test.o 

