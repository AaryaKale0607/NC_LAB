# PLAN-004: Add API Documentation with Doxygen Comments

**Created:** 2026-04-20
**Status:** Draft
**Addresses:** No inline documentation; API usage not documented; no generated HTML/PDF docs.

---

## 1. Context & motivation

The library currently lacks comprehensive API documentation. Users cannot quickly understand:
- What each method does
- Expected input ranges
- Return value meaning
- Example usage

This plan adds Doxygen-compatible inline comments that can generate HTML/PDF documentation automatically.

---

## 2. Goals

- **G1:** Add Doxygen comments to all public methods and class members in `myComplex.hpp`
- **G2:** Include parameter descriptions, return value descriptions, and usage examples
- **G3:** Generate HTML documentation using Doxygen; verify no warnings
- **G4:** Create README guide for documentation generation and maintenance

---

## 3. Non-goals

- Building full software manual (just API reference)
- Video tutorials or interactive examples
- Translating documentation to multiple languages

---

## 4. Approach

### 4.1 Doxygen Comment Format

Use standard Doxygen format:
```cpp
/**
 * @brief Add two complex numbers.
 * @param c The complex number to add
 * @return A new myComplex object representing the sum
 * 
 * Example:
 * @code
 *   myComplex z1(3, 4);  // 3+4i
 *   myComplex z2(1, 2);  // 1+2i
 *   myComplex sum = z1.add(z2);  // 4+6i
 * @endcode
 */
myComplex add(myComplex c);
```

### 4.2 Documentation Scope

- Class overview and purpose
- Constructor documentation
- All public methods: description, parameters, return, examples
- Member variables: purpose and type

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Install Doxygen and verify `doxygen` command works | 10 min | — |
| 2 | Create `Doxyfile` configuration | 15 min | 1 |
| 3 | Add Doxygen comments to class definition in `myComplex.hpp` | 20 min | — |
| 4 | Add Doxygen comments to constructors | 10 min | — |
| 5 | Add Doxygen comments to `add()`, `multiply()`, `divide()` methods | 25 min | — |
| 6 | Add Doxygen comments to `conjugate()`, `norm()` methods | 15 min | — |
| 7 | Add Doxygen comments to all operators (if PLAN-003 complete) | 15 min | — |
| 8 | Generate HTML documentation; verify no warnings | 10 min | 2,3,4,5,6,7 |
| 9 | Create `docs/README.md` guide for doc generation and updates | 15 min | 8 |
| 10 | Update current_state_report.md and create REPORT-004 | 15 min | 9 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Doxygen may not be installed | Low | Low | Document installation steps; use minimal configuration |
| Comments may become outdated | Medium | Medium | Include documentation update in any code change PR checklist |

---

## 7. Success criteria

- [ ] All public methods have Doxygen comments
- [ ] `doxygen Doxyfile` generates HTML without warnings
- [ ] HTML documentation is readable and complete
- [ ] `docs/README.md` explains how to regenerate docs
- [ ] REPORT-004 documents what was added

---

## 8. References

- Doxygen official: https://www.doxygen.nl/
- `include/myComplex.hpp` — File to document
- Example Doxyfile: https://www.doxygen.nl/manual/config.html
