# PLAN-002: Add Robust Error Handling & Input Validation

**Created:** 2026-04-21
**Status:** Approved
**Addresses:** Matrix library has minimal error handling; invalid inputs silently fail or produce incorrect results.

---

## 1. Context & motivation

PLAN-001 (Unit Test Suite) will expose edge cases and error conditions. Currently, the library:
- Does not validate matrix dimensions for operations
- Does not check for singular matrices before solving
- Does not handle file I/O failures gracefully
- Assumes input data is well-formed

This plan adds production-grade error handling following C++ best practices.

---

## 2. Goals

- **G1:** Add dimension validation for all matrix operations (add, multiply, etc.); throw std::invalid_argument on mismatch.
- **G2:** Add singular matrix detection in solvers; throw exception before attempting division by near-zero.
- **G3:** Add file I/O error handling; validate file existence and format before processing.
- **G4:** Add convergence diagnostics for iterative solvers; report if max iterations exceeded.
- **G5:** Verify all PLAN-001 tests still pass; add 10+ new error handling tests.

---

## 3. Non-goals

- Custom exception hierarchy (std::exception derivatives sufficient for now)
- Logging framework (comments and error messages sufficient)
- Recovery mechanisms (exceptions terminate operation; user handles)

---

## 4. Approach

### 4.1 Validation Points

1. **Matrix operations:**
   - Add/subtract: Verify same dimensions
   - Multiply: Verify compatible dimensions (m×n * n×p = m×p)
   - Determinant: Verify square matrix only

2. **Solver operations:**
   - Check for singular/near-singular matrices
   - Verify coefficient matrix is square
   - Verify vector dimensions match

3. **File I/O:**
   - Check file exists and is readable
   - Validate data format (correct delimiters, parseable numbers)
   - Report row/column mismatch with line numbers

### 4.2 Exception Types

```cpp
// Use standard exceptions
throw std::invalid_argument("Matrix dimensions incompatible for multiply: " + msg);
throw std::runtime_error("Cannot solve: matrix is singular (det ≈ 0)");
throw std::ios_base::failure("File format invalid: expected space-separated doubles");
```

### 4.3 Validation Order

- **Early validation:** Check inputs before computation
- **Clear error messages:** Include actual vs. expected values
- **Consistent pattern:** Same approach across all modules

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Review PLAN-001 test results; identify error cases | 20 min | PLAN-001 complete |
| 2 | Add dimension checking to Matrix operations | 45 min | 1 |
| 3 | Add singular matrix detection in solvers | 45 min | 1 |
| 4 | Add file I/O error handling | 30 min | 1 |
| 5 | Add convergence diagnostics for iterative solvers | 30 min | 1 |
| 6 | Write 10+ error handling test cases | 1 hr | 2,3,4,5 |
| 7 | Verify all PLAN-001 tests still pass | 20 min | 6 |
| 8 | Create REPORT-002; update current_state_report.md | 20 min | 7 |

**Total Estimated Time:** ~4 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Exception handling breaks existing code | Medium | High | Document as breaking change; provide migration guide |
| Over-validation slows performance | Low | Low | Validation cost negligible compared to computation |
| Message formatting inconsistencies | Low | Low | Create validation utility function; reuse everywhere |

---

## 7. Success criteria

- [ ] All matrix operations validate inputs and throw on dimension mismatch
- [ ] All solvers detect singular matrices before attempting division
- [ ] File I/O provides clear error messages with line numbers
- [ ] All 96 tests pass (86 from PLAN-001 + 10 new error tests)
- [ ] Code compiles without warnings
- [ ] REPORT-002 documents all error cases

---

## 8. References

- PLAN-001 — Unit Test Suite (prerequisite)
- `src/matrix.cpp` — Core operations to validate
- `src/gaussian_solver.cpp`, `src/lu_solver.cpp` — Solvers to enhance
- C++ Exception documentation: https://en.cppreference.com/w/cpp/error/exception
