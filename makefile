CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

TARGET = hht_sequence

# Source files
SRC = HHT.cpp signal_vectors.cpp

# Libraries
LIBS = -lfftw3 -lm

# Default target 
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(TARGET) *.o *.dSYM
