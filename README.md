# COS214-testing-framework
The documentation and useage of the testing framework functions used in COS214
As this is a template there is a lot of power but we must make use of a bunch of overloading like ==

## makefile
```Makefile
CXX := g++
CXXFLAGS := -g -std=c++11

SRC := 
OBJ := $(SRC:.cpp=.o)
BIN :=testingFramework

all: $(BIN)

$(BIN):	$(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:	$(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
 
valgrind:	$(BIN)
	valgrind --leak-check=full --track-origins=yes ./$(BIN)
```

## Test suite commands
== runs equals test  
TC runs text compare between object 1 and 2  


The main idea is to automate some testing by assigning objects to it that it maintains. At any stage these items can change. It would be wise to make mementos for the items in the suites so that states can be retrieved for each suite. 

Testing functions should also be added to allow for testing to be external, for each type of test make a lhs and rhs version that runs within the suite to update values too. 