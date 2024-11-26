# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Targets
TARGET = word_counter
SOURCES = main.cpp
HEADERS = RedBlackTree.h
OBJECTS = $(SOURCES:.cpp=.o)

# Standard Build
all: $(TARGET)

# Erstelle das Hauptprogramm
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Kompiliere Objektdateien
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean-up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony Targets
.PHONY: all clean
