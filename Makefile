# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -pedantic-errors -Wall -Wextra -Werror

# the build target executable:

OBJFILES = Firefly.o main.o
TARGET = fa

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

run5:
	./$(TARGET) tests/benchmark5.txt

run7:
	./$(TARGET) tests/benchmark7.txt

run8:
	./$(TARGET) tests/benchmark8.txt

run10:
	./$(TARGET) tests/benchmark10.txt

run15:
	./$(TARGET) tests/benchmark15.txt

run20:
	./$(TARGET) tests/benchmark20.txt

run8a:
	./$(TARGET) tests/test8a.txt

run8b:
	./$(TARGET) tests/test8b.txt

run12a:
	./$(TARGET) tests/test12a.txt

run12b:
	./$(TARGET) tests/test12b.txt

run16a:
	./$(TARGET) tests/test16a.txt

run16b:
	./$(TARGET) tests/test16b.txt

run20a:
	./$(TARGET) tests/test20a.txt

run20b:
	./$(TARGET) tests/test20b.txt

run24a:
	./$(TARGET) tests/test24a.txt

run24b:
	./$(TARGET) tests/test24b.txt

clean:
	rm -f $(OBJFILES) $(TARGET)
