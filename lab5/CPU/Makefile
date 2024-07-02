# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -mavx -mfma

# Executable names
EXEC_AVX = mma.exe
EXEC_AVX_BLOCKS = mmab.exe
EXEC_BASE = mm.exe
EXEC_AVX_BLOCKS_UP = mmab_up.exe

# Source files
SRCS_AVX = MM_AVX.cpp
SRCS_AVX_BLOCKS = MM_AVX_BLOCKS.cpp
SRCS_BASE = MM_base.cpp
SRCS_AVX_BLOCKS_UP = MM_AVX_BLOCKS_UP.cpp

# Object files
OBJS_AVX = $(SRCS_AVX:.cpp=.o)
OBJS_AVX_BLOCKS = $(SRCS_AVX_BLOCKS:.cpp=.o)
OBJS_BASE = $(SRCS_BASE:.cpp=.o)
OBJS_AVX_BLOCKS_UP = $(SRCS_AVX_BLOCKS_UP:.cpp=.o)

# Default target
all: $(EXEC_AVX) $(EXEC_AVX_BLOCKS) $(EXEC_BASE) $(EXEC_AVX_BLOCKS_UP)

# Link the mma executable
$(EXEC_AVX): $(OBJS_AVX)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link the mmab executable
$(EXEC_AVX_BLOCKS): $(OBJS_AVX_BLOCKS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link the mmb executable
$(EXEC_BASE): $(OBJS_BASE)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Link the mmab_up executable
$(EXEC_AVX_BLOCKS_UP): $(OBJS_AVX_BLOCKS_UP)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(EXEC_AVX) $(EXEC_AVX_BLOCKS) $(EXEC_BASE) $(OBJS_AVX) $(OBJS_AVX_BLOCKS) $(OBJS_BASE) $(EXEC_AVX_BLOCKS_UP) $(OBJS_AVX_BLOCKS_UP)

# Phony targets
.PHONY: all clean
