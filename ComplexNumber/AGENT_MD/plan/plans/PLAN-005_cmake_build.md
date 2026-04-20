# PLAN-005: Create CMake Build System

**Created:** 2026-04-20
**Status:** Draft
**Addresses:** No standardized build system; manual compilation; not scalable for testing and distribution.

---

## 1. Context & motivation

The project currently lacks a standardized build system. Users must manually compile with g++/clang. This makes it difficult to:
- Build unit tests (PLAN-001)
- Integrate with CI/CD pipelines
- Distribute the library to other projects
- Support multiple compilers and platforms

CMake is the industry standard for C++ projects.

---

## 2. Goals

- **G1:** Create `CMakeLists.txt` for main library and test targets
- **G2:** Support multiple compilers (g++, clang, MSVC) and platforms (Linux, macOS, Windows)
- **G3:** Enable `cmake --build . && cmake --build . --target test` workflow
- **G4:** Support install target: `make install` copies headers and library to system paths

---

## 3. Non-goals

- Package managers (Conan, vcpkg) — can add later
- Custom build flags or optimization profiles — use defaults
- Pre-built binaries distribution

---

## 4. Approach

### 4.1 CMake Structure

```
CMakeLists.txt                    # Root
├── src/CMakeLists.txt            # Library build rules
├── include/                       # Headers (no CMake needed)
└── tests/CMakeLists.txt          # Test build rules
```

### 4.2 Key Targets

- `mycomplex` — static library target
- `complex_tests` — test executable (depends on Google Test)
- `install` — install library and headers

### 4.3 Minimal Configuration

```cmake
cmake_minimum_required(VERSION 3.10)
project(mycomplex)

# Library
add_library(mycomplex src/myComplex.cpp)
target_include_directories(mycomplex PUBLIC include)

# Tests (if Google Test available)
enable_testing()
add_subdirectory(tests)
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Create root `CMakeLists.txt` with basic library setup | 20 min | — |
| 2 | Create `src/CMakeLists.txt` for library compilation | 15 min | 1 |
| 3 | Add test target (create `tests/CMakeLists.txt`) | 20 min | PLAN-001 complete |
| 4 | Configure install target (headers + library) | 15 min | 2 |
| 5 | Test build workflow: `cmake -B build && cmake --build build` | 15 min | 1,2,3,4 |
| 6 | Test `make test` command; verify all tests pass | 10 min | 5 |
| 7 | Document CMake usage in `BUILD.md` | 15 min | 6 |
| 8 | Update current_state_report.md and create REPORT-005 | 15 min | 7 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| CMake version compatibility issues | Low | Low | Specify `cmake_minimum_required(VERSION 3.10)` for broad support |
| Google Test CMake integration may be complex | Medium | Medium | Use `find_package()` or `FetchContent`; document setup |

---

## 7. Success criteria

- [ ] `cmake -B build` succeeds on Linux, macOS (and ideally Windows)
- [ ] `cmake --build build` compiles library without errors
- [ ] `cmake --build build --target test` runs all tests
- [ ] `make install` installs headers and library to `/usr/local/` or equivalent
- [ ] `BUILD.md` documents how to build and test
- [ ] REPORT-005 confirms all goals met

---

## 8. References

- CMake documentation: https://cmake.org/cmake/help/latest/
- `CMakeLists.txt` best practices: https://cliutils.gitlab.io/modern-cmake/
- Google Test CMake integration: https://github.com/google/googletest/tree/main/googletest
