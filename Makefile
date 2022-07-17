# Specify the g++ compiler
CXX := g++
# Specify the essential flags used in every build
CXXFLAGS := -Wall -Werror -std=c++20

# Header directory
INCDIR := ./include
# Driver directory
SRCDIR := ./driver
# Build directory
BLDDIR := ./build
# Install directory
INSDIR := /usr/local/include/opendsa

# Creates a list of driver files using shell command 'find'
SRC := $(shell find $(SRCDIR) -name '*.cpp')
# Create a list of headers from $(INCDIR)
HDR := $(shell find $(INCDIR) -name '*.h')
# Creates a list of object files substituted from $(SRC)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.o, $(SRC))
# Create a list of executables substituted from $(OBJ)
EXC := $(patsubst $(BLDDIR)/%.o, $(BLDDIR)/%, $(OBJ))
# Create a list of installed headers
INS := $(patsubst $(INCDIR)/%.h, $(INSDIR)/%, $(HDR))

all: main

.PHONY: clean

check-leak: main
	@echo "========= Memory leak check with Valgrind ========="
	valgrind --track-origins=yes --leak-check=full  --error-exitcode=1 -q ./main
	@echo "\nValgrind has returned a status code of $$?. No memory leak found"

uninstall:
	rm -rf /usr/local/include/opendsa/*
	rm -rf /usr/local/include/opendsa

install: $(INS)

$(INSDIR)/%: $(INCDIR)/%.h
	if [ ! -d "/usr/local/include/opendsa" ]; then \
		mkdir -p /usr/local/include/opendsa; \
	fi
	install $< $@

# Build driver programs
build: $(EXC)

$(BLDDIR)/%: $(BLDDIR)/%.o
	$(CXX) -o $@ $<

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp
	if [ ! -d "./build" ]; then \
		mkdir -p build; \
	fi
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -O0 -c -o $@ $<

main: main.o
	$(CXX)  -o main main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c -o main.o main.cpp

clean:
	rm -rf main *.o build/
