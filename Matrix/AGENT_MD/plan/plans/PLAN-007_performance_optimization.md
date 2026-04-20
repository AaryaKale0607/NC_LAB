# PLAN-007: Performance Optimization & Profiling

**Created:** 2026-04-21
**Status:** Draft
**Addresses:** No profiling or optimization; unknown performance characteristics; potential for significant speedup.

---

## 1. Context & motivation

With comprehensive testing and error handling in place (PLANS 001-006), we can now optimize performance:
- Profile solver implementations to identify bottlenecks
- Enable compiler optimizations (-O2 / -O3)
- Optimize hot paths in matrix operations
- Benchmark against reference implementations

---

## 2. Goals

- **G1:** Profile code to identify bottlenecks in matrix operations and solvers.
- **G2:** Enable compiler optimizations and measure performance improvement.
- **G3:** Optimize hot paths: matrix multiplication, forward/backward substitution.
- **G4:** Create performance benchmark suite comparing solver implementations.
- **G5:** Document performance characteristics (time, memory, scalability).

---

## 3. Non-goals

- GPU acceleration or vectorization (SIMD) — defer to v2.0
- Parallel/multi-threaded solvers — defer to v2.0
- Algorithm replacement — keep current algorithms
- Distributed computing

---

## 4. Approach

### 4.1 Profiling Tools

- **Linux/macOS:** `perf` (performance counters), `valgrind --tool=callgrind`
- **Compiler:** `-pg` flag for gprof profiling
- **Custom benchmarking:** Timer-based measurement in test suite

### 4.2 Optimization Levels

1. **Compiler flags:** Test `-O2` and `-O3`; measure impact
2. **Code optimization:** Cache locality, loop unrolling, branch prediction
3. **Algorithm tuning:** Pivot selection, convergence criteria, stopping conditions

### 4.3 Benchmark Suite

Create benchmarks for:
- Matrix-matrix multiplication (different sizes: 10×10, 50×50, 100×100, 500×500)
- Gaussian elimination (well-conditioned and ill-conditioned matrices)
- LU factorization (all 3 variants)
- Iterative solvers (convergence speed comparison)
- Lagrange interpolation (10, 20, 50 points)

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | Set up profiling environment and tools | 30 min | — |
| 2 | Profile matrix multiplication and key loops | 45 min | 1 |
| 3 | Profile Gaussian elimination and LU factorization | 45 min | 1 |
| 4 | Profile iterative solvers | 30 min | 1 |
| 5 | Identify optimization opportunities | 30 min | 2,3,4 |
| 6 | Implement compiler optimizations; measure impact | 30 min | 5 |
| 7 | Create benchmark suite for performance tracking | 1 hr | 6 |
| 8 | Document performance characteristics in REPORT-007 | 20 min | 7 |

**Total Estimated Time:** ~4.5 hours

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Optimization might introduce bugs | Medium | Medium | All tests must pass after optimization; regression testing required |
| Profiling overhead significant | Low | Low | Use sampling profiler (perf) rather than instrumentation |
| Platform-specific optimizations | Low | Low | Focus on portable optimizations; document platform differences |

---

## 7. Success criteria

- [ ] Profiling identifies bottlenecks (>20% time spent in specific functions)
- [ ] Compiler optimization flag chosen based on testing (-O2 vs -O3)
- [ ] Benchmark suite created and baseline established
- [ ] All 149+ tests pass after optimizations
- [ ] REPORT-007 documents performance improvements and characteristics

---

## 8. References

- Linux Profiling guide: https://www.linux.com/training-tutorials/
- Valgrind manual: https://valgrind.org/docs/
- Google Benchmark: https://github.com/google/benchmark
