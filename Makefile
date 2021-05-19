#
# Makefile for vsopc
# by Antoine Boonen
#

TARGET = vsopc
DIR = lib/
FOLDER = vsopcompiler/

CC = clang++
CFLAGS = -Wall -Wextra -Wshadow -Wmissing-prototypes -std=c++14
LFLAGS = `llvm-config --cxxflags --ldflags --libs core`

OBJ = symbol_table.o generator.o node.o token.o error.o checker.o parser.o scanner.o main.o 

.PHONY: install-tools clean deep-clean brew-bison
all: install-tools $(TARGET)

###################### Tool installer #######################

install-tools:
	mkdir -p /tmp/vsopc
	clang -c runtime/object.c -o /tmp/vsopc/object.o
	clang -c runtime/external.c -o /tmp/vsopc/external.o

########################## Linker ###########################

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) -o $(TARGET) $(OBJ)


######################### Compiler ##########################

$(DIR)parser.cpp: $(DIR)parser.yy
	bison -v -d --output=$(DIR)parser.cpp $(DIR)parser.yy

main.o : $(DIR)main.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: $(DIR)%.cpp $(DIR)%.hpp 
	$(CC) $(CFLAGS) -c -o $@ $<


############### Cleaners and submission rules ###############

clean:
	rm -f *.o $(TARGET)
	rm -fdr /tmp/vsopc

deep-clean: clean
	rm -f $(DIR)parser.cpp $(DIR)parser.hpp $(DIR)parser.output $(DIR)location.hh


####################### Special rules #######################

# Uses Homebrew version of Bison because MacOS one is not up-to-date
brew-bison:
	/usr/local/opt/bison/bin/bison -v -d --output=$(DIR)parser.cpp $(DIR)parser.yy
