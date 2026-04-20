# PLAN-006: Add Comprehensive API Documentation with Doxygen

**Created:** 2026-04-21
**Status:** Draft
**Addresses:** No Doxygen comments in code; API usage not documented; difficult for new users to understand library.

---

## 1. Context & motivation

The library currently lacks formal API documentation. Users must:
- Read source code to understand method signatures
- Guess at parameter meanings and return values
- Manually inspect function implementations for algorithm details

This plan adds Doxygen-compatible comments and generates HTML/PDF documentation.

---

## 2. Goals

- **G1:** Add Doxygen comments to all public methods in all headers.
- **G2:** Include parameter descriptions, return value semantics, and example usage for each method.
- **G3:** Generate HTML documentation; verify zero warnings.
- **G4:** Create developer guide documenting solver selection criteria and numerical considerations.
- **G5:** Create user guide with quick-start examples.

---

## 3. Non-goals

- Video tutorials or interactive examples
- Multiple language support
- Advanced Doxygen features (dependency graphs, call trees) — if time permits, include

---

## 4. Approach

### 4.1 Doxygen Comment Format

```cpp
/**
 * @brief Add two matrices element-wise.
 * 
 * @param other The matrix to add to this matrix.
 * @return A new Matrix containing the sum. Dimensions must match.
 * @throw std::invalid_argument if dimensions are incompatible.
 * 
 * @note Uses element-wise addition: C[i][j] = A[i][j] + B[i][j]
 * 
 * Example:
 * @code
 *   Matrix A(3, 3), B(3, 3);
 *   // ... populate A and B ...
 *   Matrix C = A.add(B);
 * @endcode
 * 
 * @see subtract(), multiply()
 */
Matrix add(const Matrix &other) const;
```

### 4.2 Documentation Scope

- **Class overview:** Purpose, intended use cases, limitations
- **All public methods:** Brief description, parameters, return value, exceptions, examples
- **Solver classes:** Algorithm description, convergence conditions, recommended use cases
- **Global functions:** Similar structure to methods

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Install Doxygen; create `Doxyfile` configuration | 15 min | — |
| 2 | Add Doxygen comments to Matrix class (header and helpers) | 45 min | 1 |
| 3 | Add Doxygen comments to Gaussian solver | 20 min | 1 |
| 4 | Add Doxygen comments to LU solver variants | 30 min | 1 |
| 5 | Add Doxygen comments to iterative solvers | 25 min | 1 |
| 6 | Add Doxygen comments to other modules (Gerschgorin, Lagrange, LS) | 30 min | 1 |
| 7 | Generate HTML documentation; verify zero warnings | 10 min | 2-6 |
| 8 | Create `docs/USER_GUIDE.md` with quick-start examples | 30 min | 7 |
| 9 | Create `docs/DEVELOPER_GUIDE.md` with solver selection criteria | 30 min | 7 |
| 10 | Update REPORT-006 and current_state_report.md | 15 min | 8,9 |

**Total Estimated Time:** ~3.5 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Doxygen version incompatibility | Low | Low | Use widely-supported features; document version requirement |
| Comments become outdated | Medium | Medium | Incorporate doc updates into code review checklist |
| Time-consuming to document everything | Medium | Low | Prioritize public methods first; internal functions can be added incrementally |

---

## 7. Success criteria

- [ ] All public methods have Doxygen comments
- [ ] `doxygen Doxyfile` generates HTML without warnings
- [ ] HTML documentation is readable and searchable
- [ ] USER_GUIDE.md provides quick-start examples
- [ ] DEVELOPER_GUIDE.md explains solver selection
- [ ] REPORT-006 documents coverage and completeness

---

## 8. References

- Doxygen official: https://www.doxygen.nl/
- `include/*.hpp` — Files to document
- Example Doxyfile: https://www.doxygen.nl/manual/config.html
- Doxygen Quick Start: https://www.doxygen.nl/manual/starting.html
