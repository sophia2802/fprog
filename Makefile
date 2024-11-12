# Programme und Compiler-Definitionen
TARGET = word_counter
CXX = clang++               # C++ Compiler
CXXFLAGS = -g -std=c++17 -Wall -lm

# Quelldateien und Header-Dateien
SRCS = main.cpp
DEPS = redBlackTree.hpp fileProcessor.hpp fileWriter.hpp

# Ziel-Binärdatei
all: $(TARGET)

# Kompilierungsregel für das Hauptprogramm
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean target, um alle erstellten Dateien zu entfernen
.PHONY: clean
clean:
	-rm -f $(TARGET) *.o
