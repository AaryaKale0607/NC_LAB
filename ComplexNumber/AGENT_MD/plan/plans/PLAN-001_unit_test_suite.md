# PLAN-001: Add Comprehensive Unit Test Suite

**Created:** 2026-04-20
**Status:** Approved
**Addresses:** Missing unit test suite blocks integration and prevents runtime validation of all operations.

---

## 1. Context & motivation

The Current State Report (`plan/current_state_report.md`) identified that the Complex Number Library has zero unit tests. All operations are verified only by code inspection, not by runtime testing. This is a critical blocker for:
- Integrating the library into production systems
- Confidently making future changes
- Detecting edge case bugs (division-by-zero, NaN propagation)

This is the highest-priority task before any other enhancements.

---

## 2. Goals

- **G1:** Create comprehensive unit test suite covering all core operations with >90% branch coverage.
- **G2:** Verify all 5 operations (add, multiply, divide, conjugate, norm) with normal cases, edge cases, and boundary conditions.
- **G3:** Integrate test execution into build system; `make test` should run all tests in < 5 seconds.
- **G4:** Document test execution results and coverage metrics in REPORT-001.

---

## 3. Non-goals

- Performance benchmarking (deferred to PLAN-006)
- Operator overloading (deferred to PLAN-003)
- Extended precision support (deferred to future roadmap)
- Memory leak detection tools (can add later if needed)

---

## 4. Approach

### 4.1 Test Framework Selection

Use **Google Test (gtest)** framework:
- Header-only or precompiled installation
- Excellent assertion macros and clear error messages
- Wide industry adoption; CI-friendly
- Integrates easily with CMake

### 4.2 Test Case Structure

Organize tests by operation:
1. **AddTests** — 5 test cases covering normal, zero, negative, identity operations
2. **MultiplyTests** — 6 test cases covering normal, identity (1+0i), zero product, conjugate multiplication
3. **DivideTests** — 7 test cases covering normal, near-zero denominator, inverse operation
4. **ConjugateTests** — 4 test cases covering various real/imaginary parts
5. **NormTests** — 5 test cases covering various magnitudes including edge cases

Total: **27 test cases** targeting >90% coverage.

### 4.3 Test Data Strategy

Use parameterized tests where possible to reduce code duplication:
- Test normal complex numbers (e.g., 3+4i, 1+1i)
- Test boundary values (0, very small floats, very large floats)
- Test special cases (pure real, pure imaginary, zero)

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Install Google Test framework and verify compilation | 20 min | — |
| 2 | Create test file structure and first test case | 15 min | 1 |
| 3 | Write 5 test cases for `add()` method | 45 min | 2 |
| 4 | Write 6 test cases for `multiply()` method | 1 hr | 2 |
| 5 | Write 7 test cases for `divide()` method (including edge cases) | 1.25 hr | 2 |
| 6 | Write 4 test cases for `conjugate()` method | 30 min | 2 |
| 7 | Write 5 test cases for `norm()` method | 45 min | 2 |
| 8 | Create Makefile test target and CMakeLists.txt if needed | 30 min | 1 |
| 9 | Run full test suite and generate coverage report | 20 min | 3,4,5,6,7 |
| 10 | Document results in REPORT-001 and update current_state_report.md | 20 min | 9 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Google Test framework incompatible with existing build | Medium | Medium | Test with simple compilation before starting; fall back to Catch2 if needed |
| Division-by-zero edge case not well-defined | High | Low | Document behavior in test; file PLAN-002 for fix if problematic |
| Coverage target (90%) may be unrealistic for current code | Low | Low | If <90%, document why; adjust goal based on analysis |
| Float precision issues in comparisons | Medium | Low | Use `ASSERT_FLOAT_EQ()` with tolerance; document assumptions |

---

## 7. Success criteria

- [ ] All 27 test cases pass with 0 failures
- [ ] Branch coverage report shows ≥90% coverage
- [ ] Test execution completes in <5 seconds on standard hardware
- [ ] `make test` target works reliably
- [ ] REPORT-001 completed with all metrics documented
- [ ] current_state_report.md updated with test results and metrics

---

## 8. References

- `AGENT_MD/spec.md` — Success Criteria §, mentions testing as requirement
- `AGENT_MD/plan/current_state_report.md` — §4 Test Suite Status, §6 Known Issues
- `include/myComplex.hpp` — API to test
- `src/myComplex.cpp` — Implementation details
- Google Test docs: https://google.github.io/googletest/
