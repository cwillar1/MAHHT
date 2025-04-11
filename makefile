CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

TARGET = hht_sequence


# Source files
SRC = HHT.cpp


# Default target
all: $(TARGET)


# Build the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) 

# Clean up build artifacts
clean:
	rm -f $(TARGET) 
	rm -f *.o
	rm -f *.dSYM

