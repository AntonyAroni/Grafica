CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lGL -lGLU -lglut

all: ejercicio1_2 ejercicio1_3 ejercicio1_4 ejercicio2_2 ejercicio2_3 ejercicio3_1 ejercicio3_2 ejercicio3_3

ejercicio1_2: ejercicio1_2.cpp
	$(CXX) $(CXXFLAGS) -o ejercicio1_2 ejercicio1_2.cpp $(LDFLAGS)

ejercicio1_3: ejercicio1_3.cpp
	$(CXX) $(CXXFLAGS) -o ejercicio1_3 ejercicio1_3.cpp $(LDFLAGS)

ejercicio1_4: ejercicio1_4.cpp
	$(CXX) $(CXXFLAGS) -o ejercicio1_4 ejercicio1_4.cpp $(LDFLAGS)

ejercicio2_2: ejercicio2_2.cpp
	$(CXX) $(CXXFLAGS) -o ejercicio2_2 ejercicio2_2.cpp $(LDFLAGS)

ejercicio2_3: ejercicio2_3.cpp
	$(CXX) $(CXXFLAGS) -o ejercicio2_3 ejercicio2_3.cpp $(LDFLAGS)

ejercicio3_1: ejercicio3_1.cpp matriz_manual.h
	$(CXX) $(CXXFLAGS) -o ejercicio3_1 ejercicio3_1.cpp $(LDFLAGS)

ejercicio3_2: ejercicio3_2.cpp matriz_manual.h
	$(CXX) $(CXXFLAGS) -o ejercicio3_2 ejercicio3_2.cpp $(LDFLAGS)

ejercicio3_3: ejercicio3_3.cpp matriz_manual.h
	$(CXX) $(CXXFLAGS) -o ejercicio3_3 ejercicio3_3.cpp $(LDFLAGS)

clean:
	rm -f ejercicio1_2 ejercicio1_3 ejercicio1_4 ejercicio2_2 ejercicio2_3 ejercicio3_1 ejercicio3_2 ejercicio3_3
