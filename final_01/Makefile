CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

TARGET = main
SRC = main.cpp algorithms.cpp shape.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
