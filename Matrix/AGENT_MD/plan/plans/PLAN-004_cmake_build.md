# PLAN-004: Create CMake Build System & Standardize Build Process

**Created:** 2026-04-21
**Status:** Approved
**Addresses:** Current Makefile-based build is not scalable; CMake enables CI/CD, multi-platform support, and test integration.

---

## 1. Context & motivation

The current Makefile works but is inflexible:
- No standard test target integration
- Hard to port to Windows/MSVC
- Difficult to integrate with CI/CD pipelines
- Not suitable for library distribution

CMake is industry standard for C++ projects and enables:
- Cross-platform builds (Linux, macOS, Windows)
- Easy Google Test integration
- Package export for external consumers
- CI/CD pipeline integration (GitHub Actions, Azure Pipelines)

---

## 2. Goals

- **G1:** Create `CMakeLists.txt` for library and test targets supporting Linux, macOS, Windows.
- **G2:** Enable `cmake -B build && cmake --build build && cmake --build build --target test` workflow.
- **G3:** Support install target: `cmake --install` copies headers and library to system paths.
- **G4:** Maintain backward compatibility: existing source files unchanged.
- **G5:** Reduce build time and support parallel compilation with `cmake --build . -j 4`.

---

## 3. Non-goals

- Conan/vcpkg package manager integration (future v1.1)
- Pre-built binaries or installer generation
- Custom build flags or profiles

---

## 4. Approach

### 4.1 CMake Structure

```
CMakeLists.txt                    # Root project config
├── src/CMakeLists.txt            # Library build rules
├── tests/CMakeLists.txt          # Test build rules
└── include/                       # Headers (no CMake needed)
```

### 4.2 Key Targets

- `matrix` — static and shared libraries
- `matrix_tests` — test executable (depends on Google Test)
- `install` — install library and headers
- `test` — run all tests via `ctest`

### 4.3 Configuration

```cmake
cmake_minimum_required(VERSION 3.15)
project(matrix VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library target
add_library(matrix src/matrix.cpp src/gaussian_solver.cpp ...)
target_include_directories(matrix PUBLIC include)

# Test target
enable_testing()
find_package(GTest REQUIRED)
add_executable(matrix_tests tests/test_matrix.cpp)
target_link_libraries(matrix_tests matrix GTest::gtest_main)
add_test(NAME matrix COMMAND matrix_tests)
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Create root `CMakeLists.txt` with basic library setup | 20 min | — |
| 2 | Create `src/CMakeLists.txt` for library compilation | 15 min | 1 |
| 3 | Add Google Test integration (create `tests/CMakeLists.txt`) | 20 min | 1 |
| 4 | Configure install target (headers + library) | 15 min | 2 |
| 5 | Test build workflow: `cmake -B build && cmake --build build` | 15 min | 1,2,3,4 |
| 6 | Test on Linux, macOS (verify portability) | 20 min | 5 |
| 7 | Create `BUILD.md` documentation | 15 min | 6 |
| 8 | Update current_state_report.md; create REPORT-004 | 15 min | 7 |

**Total Estimated Time:** ~2 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| CMake version compatibility | Low | Low | Require 3.15+ (2019 release); broad support |
| Complex library dependencies | Low | Low | No external deps beyond STL; setup trivial |
| Build time regression | Low | Low | CMake similar to Makefile; monitor with timing |

---

## 7. Success criteria

- [ ] `cmake -B build` succeeds on Linux, macOS
- [ ] `cmake --build build` compiles all sources
- [ ] `cmake --build build --target test` runs all tests
- [ ] `cmake --install` installs headers and library
- [ ] `BUILD.md` documents workflow
- [ ] Backward compatibility: all tests pass (same as Makefile build)

---

## 8. References

- Current Makefile: `Makefile`
- CMake documentation: https://cmake.org/cmake/help/latest/
- Google Test CMake: https://github.com/google/googletest
- Modern CMake: https://cliutils.gitlab.io/modern-cmake/
