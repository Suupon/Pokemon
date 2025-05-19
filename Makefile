CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
INCLUDE = -Iinclude
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
EXECUTABLE = pokemon

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run 