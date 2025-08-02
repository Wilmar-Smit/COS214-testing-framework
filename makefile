CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

SRC = main.cpp Logger.cpp CLI.cpp CLIConstants.cpp array.cpp testing.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)

leaks: all
	leaks --atExit -- ./$(EXEC) || true

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)
