SRC = source/compiler/*.cpp

CC = g++
FLAGS = -std=c++17 -O2
OUTPUT = hc4
LIBS = -lShell32

all: $(OUTPUT).exe
	$(CC) -O2 source/main.cpp hc4.a -o $^
	@clear
	./$^ examples/event/main.hc4 -debug

build: $(SRC)
	@rm -f $(OUTPUT).a
	$(CC) $(FLAGS) -c $^ -c $(LIBS)
	ar -qc $(OUTPUT).a *.o
	make all