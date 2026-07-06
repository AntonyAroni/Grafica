CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3 -I./glm -I.
LDFLAGS = -lGLEW -lGL -lglfw

SOURCES = main.cpp shader.cpp camera.cpp rain_system.cpp texture.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
