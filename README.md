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

## Textfile format
outlines tests done per-suite 
several tests will be put in currently only 
==

```plain

TS[1]
== <will run the == test>
```
