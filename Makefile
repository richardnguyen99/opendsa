CXX := g++
CXXFLAGS := -Wall -Werror -std=c++2a

all: main

check-leak: main
	@echo "========= Memory leak check with Valgrind ========="
	valgrind --track-origins=yes --leak-check=full  --error-exitcode=1 -q ./main
	@echo "\nValgrind has returned a status code of $$?. No memory leak found"

main: main.o
	$(CXX)  -o main main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

clean:
	rm -rf main *.o
