CC = g++
CFLAGS = -Wall -std=c++11 -g
LDFLAGS = -lGL -lGLU -lglut

SOURCES = main.cpp camera.cpp texture.cpp light.cpp scene.cpp ui.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
