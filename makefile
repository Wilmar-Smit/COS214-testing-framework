
CXX := g++
CXXFLAGS := -g -std=c++11

SRC := main.cpp  #<cpp files to run> do not put testing.cpp here
OBJ := $(SRC:.cpp=.o)
BIN := TestingFramework

all: $(BIN)

$(BIN):	$(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run r:	$(BIN)
	./$(BIN)

clean c:
	rm -f $(OBJ) $(BIN)
 
valgrind v:	$(BIN)
	valgrind --leak-check=full --track-origins=yes ./$(BIN)
