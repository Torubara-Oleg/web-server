# Compiler
CXX = g++

# Compiler flags
# -std=c++20: Use C++20 standard (required for std::source_location)
# -O3: Optimize for speed
# -Wall: Enable all compiler warnings
# -I.: Include current directory
# -Ilogger: Include logger directory
# -Ilayers/http: Include http layer directory
# -Ilayers/tcp: Include tcp layer directory
# -Iutils: Include utils directory
# -Iutils/filesystem: Include filesystem utils directory
# -Iutils/net: Include net utils directory
CXXFLAGS = -DDEBUG -std=c++2a -O3 -Wall -I. -Ilogger -Ilayers/http -Ilayers/tcp -Iutils -Iutils/filesystem -Iutils/net

# Linker flags
# -lpthread: Link against the pthread library
LDFLAGS = -lpthread

# Executable name
TARGET = server

# Source files
# Recursively find source files in specific directories
SOURCES = $(wildcard *.cpp) \
          $(wildcard logger/*.cpp) \
          $(wildcard layers/http/*.cpp) \
          $(wildcard layers/tcp/*.cpp) \
          $(wildcard utils/filesystem/*.cpp) \
          $(wildcard utils/*.cpp)

# Object files
# Replace .cpp extension with .o for all source files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	rm -f $(OBJECTS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
