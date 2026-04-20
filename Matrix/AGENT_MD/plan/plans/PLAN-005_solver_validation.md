# PLAN-005: Validate All Solver Implementations with Comprehensive Test Matrices

**Created:** 2026-04-21
**Status:** Approved
**Addresses:** Solver correctness verified only through CLI manual testing; need systematic validation with known solutions.

---

## 1. Context & motivation

PLAN-001 creates unit tests, but solver validation needs specific attention:
- Each solver algorithm (Gaussian, Crout, Doolittle, Cholesky, Jacobi, Seidel) must produce identical correct results
- Convergence behavior must be predictable and documented
- Ill-conditioned and edge case matrices must be handled correctly
- Numerical accuracy must meet specified tolerances

This plan creates a comprehensive solver validation suite.

---

## 2. Goals

- **G1:** For each solver, validate correctness against known solutions (hand-calculated, verified software).
- **G2:** Test all solvers on identical test matrices; verify numerical equivalence (tolerance 1e-10).
- **G3:** Validate iterative solver convergence: appropriate tolerance and max iterations.
- **G4:** Test edge cases: singular matrices, very small/large values, diagonal dominant / non-dominant.
- **G5:** Document solver accuracy, convergence rate, and recommended use cases.

---

## 3. Non-goals

- Performance profiling (deferred to PLAN-007)
- Solver comparison/benchmarking
- Parallel solver implementations

---

## 4. Approach

### 4.1 Test Matrix Library

Create reusable test matrices:
1. **Identity matrix** I_n: Standard solution, determinant = 1
2. **Diagonal matrix** D: Well-conditioned, easy to verify
3. **Symmetric positive definite (SPD)** for Cholesky
4. **Hilbert matrix** — notoriously ill-conditioned; tests numerical stability
5. **Random well-conditioned** matrix: Realistic use case
6. **Nearly singular** matrix: Tests boundary conditions
7. **Diagonally dominant** matrix: Tests Jacobi/Seidel convergence conditions

### 4.2 Validation Method

For each test matrix A and RHS b:
1. Solve Ax = b using each solver
2. Compute residual: r = b - A*x
3. Verify ||r|| < tolerance (1e-10 for direct solvers, 1e-6 for iterative)
4. For iterative solvers, track convergence rate

### 4.3 Convergence Analysis

For iterative solvers:
- Spectral radius of iteration matrix
- Convergence rate (linear, superlinear)
- Diagonal dominance check
- Condition number effect

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Create test matrix generation functions | 30 min | — |
| 2 | Create solver validation framework and residual computation | 30 min | 1 |
| 3 | Create Gaussian solver validation tests (5 test cases) | 45 min | 2 |
| 4 | Create LU solver validation tests (15 test cases for 3 methods) | 1 hr | 2 |
| 5 | Create iterative solver validation tests (10 test cases) | 1 hr | 2 |
| 6 | Create edge case/ill-conditioned tests (8 test cases) | 45 min | 2 |
| 7 | Document solver accuracy and convergence characteristics | 30 min | 3,4,5,6 |
| 8 | Create REPORT-005; update current_state_report.md | 20 min | 7 |

**Total Estimated Time:** ~5 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Ill-conditioned matrices may fail | Medium | Medium | Document which solvers handle poorly; note limitations |
| Iterative solver convergence varies | Medium | Low | Test with diagonally dominant matrices; document assumptions |
| Cholesky on non-SPD matrices | High | Medium | Add SPD verification before Cholesky; throw exception if not SPD |

---

## 7. Success criteria

- [ ] All solvers pass 38 validation tests (5+15+10+8)
- [ ] Residuals within tolerance for all solvers
- [ ] Convergence behavior documented for iterative solvers
- [ ] Ill-conditioned matrix handling understood and documented
- [ ] REPORT-005 includes solver accuracy matrix
- [ ] All prior tests (111) still pass

---

## 8. References

- PLAN-001 — Unit Test Suite (foundation)
- PLAN-002 — Error handling (prerequisite for robust validation)
- Numerical Recipes: https://numerical.recipes/
- MATLAB matrix documentation: https://www.mathworks.com/
