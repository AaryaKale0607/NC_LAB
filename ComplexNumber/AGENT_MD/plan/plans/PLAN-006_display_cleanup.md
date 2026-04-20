# PLAN-006: Implement Display Method & Fix Commented Code

**Created:** 2026-04-20
**Status:** Draft
**Addresses:** `display()` method is commented out; cleanup and implementation needed.

---

## 1. Context & motivation

The `myComplex` class has a commented-out `display()` method. This should either be:
1. Implemented (if needed for testing/demos)
2. Removed (if obsolete)

This plan clarifies the status and decides on a path forward.

---

## 2. Goals

- **G1:** Review commented `display()` method and determine its purpose
- **G2:** Either implement it properly with formatted output, or remove it entirely
- **G3:** If implemented, add stream operator `operator<<` as the preferred output method (see PLAN-003)
- **G4:** Clean up all commented code from the project

---

## 3. Non-goals

- Implementing console coloring or advanced formatting

---

## 4. Approach

### 4.1 Decision Process

1. Check if `display()` is called anywhere in codebase
2. Check `main.cpp` to see if demos need it
3. Decide: remove or reimplement

### 4.2 If Reimplement

Use stream operator instead (modern C++):
```cpp
std::ostream& operator<<(std::ostream& os, const myComplex& c) {
    os << c.a << " + " << c.b << "i";
    return os;
}
```

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Search codebase for any calls to `display()` | 5 min | — |
| 2 | Review `main.cpp` and test code for display usage | 10 min | — |
| 3 | Decide: remove or reimplement based on findings | 10 min | 1,2 |
| 4 | If remove: delete `display()` from both header and source | 5 min | 3 |
| 5 | If reimplement: implement via `operator<<` | 15 min | 3 |
| 6 | Update tests if needed; verify no breakage | 10 min | 4 or 5 |
| 7 | Update current_state_report.md and create REPORT-006 | 15 min | 6 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Removal breaks user code | Low | Medium | Document breaking change; provide migration guide |
| Reimplementation edge cases | Low | Low | Test with various complex numbers |

---

## 7. Success criteria

- [ ] Decision made and documented (remove vs. reimplement)
- [ ] All commented code removed or properly implemented
- [ ] No compilation warnings
- [ ] All tests still pass
- [ ] REPORT-006 documents the decision and changes

---

## 8. References

- `src/myComplex.cpp` — Contains commented `display()` method
- `include/myComplex.hpp` — Header declaration
- PLAN-003 — Implements `operator<<` (preferred approach)
