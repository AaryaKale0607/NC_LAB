# PLAN-003: Implement Operator Overloading for Matrix Expressions

**Created:** 2026-04-21
**Status:** Draft
**Addresses:** Current API requires method calls (e.g., `A.add(B)`); C++ developers expect operator syntax (e.g., `A + B`).

---

## 1. Context & motivation

The Matrix class currently requires verbose method-based API:
- `C = A.add(B)` instead of `C = A + B`
- `C = A.multiply(B)` instead of `C = A * B`
- `M.display()` for output instead of `std::cout << M`

Operator overloading makes the API more intuitive and enables natural mathematical expressions like `C = (A + B) * (A - B)`.

---

## 2. Goals

- **G1:** Implement arithmetic operators: `+`, `-`, `*` with proper dimensions checking.
- **G2:** Implement comparison operators: `==`, `!=` (with floating-point epsilon tolerance).
- **G3:** Implement unary operators: `-` (negation), `~` (transpose, optional).
- **G4:** Implement stream I/O operators: `<<`, `>>` for console and file access.
- **G5:** Maintain backward compatibility with existing method-based API.
- **G6:** Verify all prior tests pass; add operator-specific tests.

---

## 3. Non-goals

- Compound assignment operators (`+=`, `-=`, `*=`, `/=`) — defer to v1.1
- Matrix element access via `operator[][]` (requires proxy objects; keep simple)
- Implicit type conversions

---

## 4. Approach

### 4.1 Operator Implementation Strategy

**Member operators (for left-hand modification):**
- Transpose, negation: Can be member operators
- Comparison: Should be members or friends

**Global/friend operators (for symmetric operations):**
- Addition, subtraction: Global (no need for left-hand access)
- Multiplication: Global for clarity
- Stream I/O: Friends for access to private members

### 4.2 Example Implementation

```cpp
// Global operators (in header and implementation)
Matrix operator+(const Matrix& A, const Matrix& B) {
    return A.add(B);  // Delegate to existing method
}

Matrix operator*(const Matrix& A, const Matrix& B) {
    return A.multiply(B);
}

std::ostream& operator<<(std::ostream& os, const Matrix& M) {
    M.display(os);  // Delegate to display
    return os;
}
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Add operator declarations to `matrix.hpp` | 15 min | — |
| 2 | Implement arithmetic operators (+, -, *) | 20 min | 1 |
| 3 | Implement comparison operators (==, !=) | 15 min | 1 |
| 4 | Implement unary operators (-, transpose) | 15 min | 1 |
| 5 | Implement stream I/O operators (<<, >>) | 20 min | 1 |
| 6 | Write 15 operator-specific tests | 1 hr | 2,3,4,5 |
| 7 | Verify all prior tests pass (no regressions) | 15 min | 6 |
| 8 | Update REPORT-003 and current_state_report.md | 15 min | 7 |

**Total Estimated Time:** ~2.5 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Float comparison tolerance tuning | Medium | Low | Use 1e-10 epsilon; document and test with near-equal values |
| Stream output format changes | Low | Low | Keep simple format; document design decision |
| Performance overhead from operators | Low | Low | Operators are zero-cost abstractions (delegates to methods) |

---

## 7. Success criteria

- [ ] All 7 operators implemented and tested
- [ ] All prior tests (96) pass unchanged
- [ ] 15 new operator tests all pass
- [ ] Code compiles without warnings
- [ ] Backward compatibility maintained (old method calls still work)

---

## 8. References

- `include/matrix.hpp` — Header file to extend
- `src/matrix.cpp` — Implementation
- PLAN-002 — Error handling (prerequisite for robust operators)
- C++ Operator Overloading: https://en.cppreference.com/w/cpp/language/operators
