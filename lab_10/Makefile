CC = g++
CFLAGS = -Wall -std=c++11 -O2 -I.
LDFLAGS = -lGL -lGLU -lglut

SOURCES = main.cpp boid.cpp simulation.cpp ui.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
