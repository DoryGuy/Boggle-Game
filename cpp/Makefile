
CXX=g++-13
CXX_VERSION=c++23
OPTIMIZE_FLAG=-O3
DEBUG_FLAG=-g
WARNING_FLAGS=-Wall -Wextra -Wconversion
CXXFLAGS=-std=$(CXX_VERSION) $(OPTIMIZE_FLAG) $(WARNING_FLAGS) $(DEBUG_FLAG)
INCLUDE_DIR=/usr/local/include
LIB_DIR=/usr/local/lib
RM=rm


# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cpp,%.o,$(SRCS))


# Define a pattern rule that compiles every .cpp file into a .o file
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

LIBRARIES=

# The build target
TARGET = test_program.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ -L$(LIB_DIR) $(LIBRARIES) $^

clean:
	$(RM) *.o $(TARGET)

ctags:
	ctags --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ *.cpp

.PHONY: all clean
