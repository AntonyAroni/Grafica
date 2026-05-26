CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -lGL -lGLU -lglut

SOURCES = main.cpp algorithms.cpp primitiveshape.cpp
HEADERS = algorithms.h entity.h primitiveshape.h pointshape.h lineshape.h polygonshape.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
