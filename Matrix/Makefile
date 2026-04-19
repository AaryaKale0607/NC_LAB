# Compiler
CXX      = g++
CXXFLAGS = -std=c++17 -Iinclude

# Core library sources
SRC = src/matrix.cpp src/gaussian_solver.cpp src/lu_solver.cpp \
      src/system_linear_eq.cpp src/iterative_solver.cpp \
      src/eigen.cpp src/gerschgorin.cpp src/interpolation.cpp src/lagrange.cpp \
      src/curve_fitting.cpp src/least_squares.cpp

# Interpolation sources (compiled into every executable)
#INTERP_SRC = src/interpolation.cpp src/lagrange.cpp

# Object files (library only)
OBJ = $(SRC:.cpp=.o)

# Libraries
STATIC_LIB = libmatrix.a
SHARED_LIB = libmatrix.so

# Default target
all: $(STATIC_LIB) $(SHARED_LIB) matrix_static matrix_dynamic

# Compile library object files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# Static library
$(STATIC_LIB): $(OBJ)
	ar rcs $(STATIC_LIB) $(OBJ)

# Shared library
$(SHARED_LIB): $(OBJ)
	$(CXX) -shared -o $(SHARED_LIB) $(OBJ)

# matrix_static — includes interpolation sources directly
matrix_static: main.cpp $(STATIC_LIB) $(INTERP_SRC)
	$(CXX) $(CXXFLAGS) main.cpp $(INTERP_SRC) libmatrix.a -o matrix_static

# matrix_dynamic — includes interpolation sources directly
matrix_dynamic: main.cpp $(SHARED_LIB) $(INTERP_SRC)
	$(CXX) $(CXXFLAGS) main.cpp $(INTERP_SRC) -L. -lmatrix -o matrix_dynamic

# Run
run_static:
	./matrix_static

run_dynamic:
	LD_LIBRARY_PATH=. ./matrix_dynamic

# Clean
clean:
	rm -f src/*.o $(STATIC_LIB) $(SHARED_LIB) matrix_static matrix_dynamic