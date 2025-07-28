CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = main.cpp testing.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = test_runner

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean