# PLAN-002: Add Division-by-Zero Safety & Error Handling

**Created:** 2026-04-20
**Status:** Draft
**Addresses:** `divide()` method has no safeguard for division-by-zero; produces silent NaN/Inf propagation.

---

## 1. Context & motivation

PLAN-001 (Unit Test Suite) will expose edge cases in the `divide()` method. Currently, when divisor is zero (or very close to zero), the method returns NaN or Inf without warning. This violates the principle of "fail fast, fail loud" and makes debugging difficult.

This plan addresses robustness: detect invalid operations and handle them explicitly.

---

## 2. Goals

- **G1:** Add explicit check for zero denominator in `divide()`; throw std::invalid_argument or return error indicator.
- **G2:** Validate that `divide()` gracefully handles boundary cases: near-zero denominators, very large/small results.
- **G3:** Document the error handling behavior in code comments and API docs (PLAN-004).
- **G4:** Verify all PLAN-001 tests still pass after changes; add edge case tests if needed.

---

## 3. Non-goals

- Implementing exception hierarchy (keep simple: throw std::invalid_argument)
- Adding logging framework (comments only for now)
- Modifying other operations (divide only)

---

## 4. Approach

### 4.1 Error Detection

Add denominator check before computation:
```cpp
float den = c.a * c.a + c.b * c.b;
if (den < 1e-10) {  // Threshold to detect near-zero
    throw std::invalid_argument("Division by zero or near-zero complex number");
}
```

### 4.2 Alternative: Return Code Pattern (if exceptions are not desired)

Modify signature to return `bool`:
```cpp
bool divide(myComplex c, myComplex& result);
```

Recommendation: Use exceptions (C++ standard practice) unless team has constraints.

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Review PLAN-001 test results; identify division edge cases | 15 min | PLAN-001 complete |
| 2 | Update `divide()` implementation with denominator check | 20 min | 1 |
| 3 | Update `myComplex.hpp` header (no signature change, just comments) | 10 min | 2 |
| 4 | Write 3 new test cases for error handling in `divide()` | 30 min | 2 |
| 5 | Run full test suite; verify no regressions | 15 min | 4 |
| 6 | Update REPORT-002 and current_state_report.md | 15 min | 5 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Existing code catches/ignores exceptions | Low | Medium | Search codebase for `try/catch` blocks; document breaking change |
| Exception throwing changes calling code semantics | Medium | Medium | Document in REPORT-002 with migration guide for users |
| Tests may not cover all edge cases | Low | Low | Add parameterized test with range of near-zero values |

---

## 7. Success criteria

- [ ] `divide()` throws std::invalid_argument when denominator is zero or near-zero
- [ ] All 27 tests from PLAN-001 still pass
- [ ] 3 new error-handling tests pass
- [ ] Code review (human or AI) confirms logic is correct
- [ ] REPORT-002 documents the change and testing approach

---

## 8. References

- PLAN-001 — Unit Test Suite (prerequisite; will provide test framework and initial coverage)
- `src/myComplex.cpp::divide()` — Implementation to update
- `include/myComplex.hpp` — Header comments to update
- C++ exception documentation: https://en.cppreference.com/w/cpp/error/invalid_argument
