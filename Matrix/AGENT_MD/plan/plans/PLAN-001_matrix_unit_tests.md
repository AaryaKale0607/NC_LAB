# PLAN-001: Create Comprehensive Unit Test Suite

**Created:** 2026-04-21
**Status:** Approved
**Addresses:** Missing unit test suite blocks validation of all matrix operations and solver algorithms.

---

## 1. Context & motivation

The Current State Report identifies that all matrix operations and solver implementations are verified only by manual CLI testing and code inspection. There is no automated unit test suite. This is a critical blocker for:
- Confident code refactoring
- Regression detection
- Integration into CI/CD pipelines
- Production deployment
- Detecting edge case bugs (singular matrices, ill-conditioned systems, dimension mismatches)

This is the highest-priority task before any other enhancements.

---

## 2. Goals

- **G1:** Create comprehensive unit test suite for all Matrix operations covering normal cases, edge cases, and error conditions with >90% branch coverage.
- **G2:** Implement tests for all 8 solver implementations (Gaussian, Crout, Doolittle, Cholesky, Jacobi, Seidel, Gerschgorin, Lagrange).
- **G3:** Test least squares curve fitting for all 5 model types.
- **G4:** Validate numerical accuracy and convergence behavior of iterative solvers.
- **G5:** Integrate test execution into build system; `make test` or CMake target runs all tests in <10 seconds.

---

## 3. Non-goals

- Performance optimization or benchmarking (deferred to PLAN-007)
- GUI test interface (CLI-based test runner sufficient)
- Extended precision variants (double only for now)
- Sparse matrix testing (deferred to v1.1)

---

## 4. Approach

### 4.1 Test Framework Selection

Use **Google Test (gtest)** framework:
- Comprehensive assertion macros and clear error messages
- Parameterized tests for data-driven testing
- Test fixtures for shared setup
- Wide adoption in numerical computing projects

### 4.2 Test Case Organization

Organize tests by component:
1. **MatrixTests** — Core Matrix class: addition, subtraction, multiplication (15 tests)
2. **MatrixPropertiesTests** — Matrix properties: isSquare, isSymmetric, isDiagonal, etc. (8 tests)
3. **GaussianSolverTests** — Gaussian elimination with pivoting (10 tests)
4. **LUSolverTests** — Crout, Doolittle, Cholesky methods (15 tests)
5. **IterativeSolverTests** — Gauss-Jacobi, Gauss-Seidel convergence (12 tests)
6. **GerschgorinTests** — Eigenvalue bounds validation (6 tests)
7. **LagrangeTests** — Interpolation polynomial accuracy (8 tests)
8. **LeastSquaresTests** — All 5 model fitting variants (12 tests)

**Total: ~86 test cases** targeting >90% coverage.

### 4.3 Test Data Strategy

- Use small well-known matrices (3×3, 4×4) with hand-calculated results for validation
- Include identity, diagonal, and symmetric matrices
- Test boundary conditions: 1×1, 2×2 matrices; 1×10 vectors
- Ill-conditioned matrices to expose numerical issues
- Singular matrices to test error handling

### 4.4 Test Categories

- **Correctness tests:** Validate mathematical results against known solutions
- **Edge case tests:** Empty matrices, unit matrices, singular matrices
- **Dimension mismatch tests:** Incompatible matrix sizes for operations
- **Error handling tests:** Invalid inputs, file I/O failures
- **Convergence tests:** Iterative solver tolerance and iteration limits
- **Accuracy tests:** Floating-point comparison with epsilon tolerance (1e-10)

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Install/configure Google Test framework | 20 min | — |
| 2 | Create test file structure and test framework setup | 20 min | 1 |
| 3 | Write 15 tests for Matrix core operations (add, multiply, subtract) | 1.5 hr | 2 |
| 4 | Write 8 tests for Matrix properties (isSquare, isSymmetric, isDiagonal, etc.) | 45 min | 2 |
| 5 | Write 10 tests for Gaussian elimination solver | 1.25 hr | 2 |
| 6 | Write 15 tests for LU factorization (Crout, Doolittle, Cholesky) | 1.5 hr | 2 |
| 7 | Write 12 tests for iterative solvers (Jacobi, Seidel) | 1.25 hr | 2 |
| 8 | Write 6 tests for Gerschgorin circle theorem | 45 min | 2 |
| 9 | Write 8 tests for Lagrange interpolation | 1 hr | 2 |
| 10 | Write 12 tests for least squares curve fitting (all 5 models) | 1.5 hr | 2 |
| 11 | Generate code coverage report; identify uncovered branches | 20 min | 3-10 |
| 12 | Document test results and create REPORT-001 | 30 min | 11 |

**Total Estimated Time:** ~12 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Google Test integration complexity | Medium | Medium | Use simple CMakeLists.txt setup; test with small example first |
| Floating-point precision comparison | High | Low | Use epsilon-based comparison (1e-10); document assumptions |
| Test data generation tedious | Medium | Low | Script to generate test matrices programmatically; reuse known solutions |
| Coverage target unreachable | Low | Low | If <90%, document why and adjust goals; iterative refinement acceptable |
| Build/test time explosion | Low | Medium | Run tests in parallel if available; monitor execution time |

---

## 7. Success criteria

- [ ] All 86 test cases pass with 0 failures
- [ ] Branch coverage ≥90% achieved across all modules
- [ ] Test execution completes in <10 seconds
- [ ] All test files compile without warnings
- [ ] REPORT-001 documents all metrics and findings
- [ ] No regressions in existing solver correctness

---

## 8. References

- `AGENT_MD/spec.md` — Success Criteria § (testing requirement)
- `AGENT_MD/plan/current_state_report.md` — §4 Test Suite Status, §5 Known Issues
- `include/*.hpp` — All headers to test
- `src/*.cpp` — All implementations
- Google Test documentation: https://google.github.io/googletest/
