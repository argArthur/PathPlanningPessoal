CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

ALG_SRCS = $(wildcard algorithms/*.cpp)

SRCS = $(wildcard *.cpp) $(ALG_SRCS)

OBJS = $(SRCS:.cpp=.o)

TARGET = path_planning_app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
