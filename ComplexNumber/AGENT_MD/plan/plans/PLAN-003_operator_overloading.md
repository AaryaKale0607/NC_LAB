# PLAN-003: Implement Operator Overloading for Syntax Sugar

**Created:** 2026-04-20
**Status:** Draft
**Addresses:** Current API requires verbose method calls (e.g., `z1.add(z2)`); C++ developers expect operator syntax (e.g., `z1 + z2`).

---

## 1. Context & motivation

The current API is functional but verbose. Complex number libraries in C++ (e.g., `std::complex`) provide intuitive operator overloading:
- `z1 + z2` instead of `z1.add(z2)`
- `z1 * z2` instead of `z1.multiply(z2)`
- `z1 / z2` instead of `z1.divide(z2)`

This plan improves usability without breaking existing method-based API.

---

## 2. Goals

- **G1:** Implement operator overloads: `+`, `-`, `*`, `/`, `==`, `!=`, unary `-`
- **G2:** Maintain backward compatibility with existing `add()`, `multiply()`, etc. methods
- **G3:** Support compound assignments: `+=`, `-=`, `*=`, `/=` (optional; depends on complexity)
- **G4:** Update stream I/O: `operator<<` for printing (e.g., `std::cout << z`)
- **G5:** Verify all PLAN-001 and PLAN-002 tests still pass; add operator tests.

---

## 3. Non-goals

- Implementing unary operators like `!` or `^` (out of scope)
- Support for mixed operations with `float` (e.g., `z + 2.0`) — can add later
- Implicit type conversion (keep explicit)

---

## 4. Approach

### 4.1 Operator Overloading Strategy

Add global or member functions for each operator:
- Member operators (preferred for `+=`, `-=`, etc.): `myComplex& operator+=(const myComplex& c)`
- Global operators for symmetric operations: `myComplex operator+(const myComplex& a, const myComplex& b)`

### 4.2 Implementation Plan

1. `operator+`, `operator-` — wrap existing `add()` logic
2. `operator*`, `operator/` — wrap existing `multiply()`, `divide()` logic
3. `operator==`, `operator!=` — float comparison with tolerance
4. `operator-` (unary) — negate real and imaginary parts
5. `operator<<` — stream output: `"a + bi"` format (e.g., `"3 + 4i"`)

### 4.3 Code Example

```cpp
// Header
myComplex operator+(const myComplex& a, const myComplex& b);
myComplex operator-(const myComplex& a, const myComplex& b);
std::ostream& operator<<(std::ostream& os, const myComplex& c);

// Implementation
myComplex operator+(const myComplex& a, const myComplex& b) {
    return a.add(b);
}
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Add operator declarations to `myComplex.hpp` | 15 min | — |
| 2 | Implement `operator+`, `operator-` in `myComplex.cpp` | 20 min | 1 |
| 3 | Implement `operator*`, `operator/` in `myComplex.cpp` | 20 min | 1 |
| 4 | Implement comparison operators `==`, `!=` | 15 min | 1 |
| 5 | Implement unary `operator-` | 10 min | 1 |
| 6 | Implement `operator<<` for stream output | 15 min | 1 |
| 7 | Write 12 test cases for all operators | 1 hr | 2,3,4,5,6 |
| 8 | Verify all prior tests still pass (PLAN-001, PLAN-002) | 15 min | 7 |
| 9 | Update current_state_report.md and create REPORT-003 | 15 min | 8 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Float comparison tolerance may need tuning | Medium | Low | Use epsilon-based comparison; document in code |
| Stream output format may be ambiguous | Low | Low | Test various input combinations; document format |
| Breaking method API if operators replace methods | Low | Low | Keep methods; operators are just convenience wrappers |

---

## 7. Success criteria

- [ ] All 7 operators implemented and tested
- [ ] All prior tests (PLAN-001, PLAN-002) pass unchanged
- [ ] 12 new operator tests all pass
- [ ] Code compiles without warnings
- [ ] REPORT-003 documents all operators with examples

---

## 8. References

- `include/myComplex.hpp` — Header file to extend
- `src/myComplex.cpp` — Implementation file to extend
- `std::complex` documentation: https://en.cppreference.com/w/cpp/numeric/complex
- C++ Operator Overloading guide: https://en.cppreference.com/w/cpp/language/operators
