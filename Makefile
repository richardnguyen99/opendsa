CXX := c++
CXXFLAGS := -Wall -Werror -std=c++2a

all: main

main: main.o
	$(CXX)  -o main main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

clean:
	rm -rf main *.o
