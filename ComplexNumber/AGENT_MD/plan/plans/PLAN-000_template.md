# PLAN-001: [Title — Example: Add Unit Test Suite]

**Created:** YYYY-MM-DD
**Status:** Draft
**Addresses:** _[One-line description of the gap or need this plan targets.]_

---

## 1. Context & motivation

_[Why this plan exists. Link to the current state report or prior reports that surfaced the need.]_

Example: The Current State Report (`plan/current_state_report.md`) identified that the Complex Number Library has no unit test suite. All operations are verified only by code inspection, not runtime testing. This is a blocker for further development and integration.

---

## 2. Goals

- **G1:** _[Measurable outcome — e.g., "Create comprehensive unit test suite with >90% branch coverage."]_
- **G2:** _[Measurable outcome — e.g., "All core operations (add, multiply, divide, conjugate, norm) have passing tests."]_
- **G3:** _[Measurable outcome — e.g., "Test suite runs successfully on the current build environment."]_

---

## 3. Non-goals

- _[What's explicitly out of scope — e.g., "Performance benchmarking is deferred to PLAN-002."]_
- _[Prevents scope creep and sets expectations for AI agents.]_

---

## 4. Approach

_[Detailed description of the technical approach. Include architecture decisions, trade-offs, and alternatives considered.]_

### 4.1 _[Subsection — e.g., "Test framework selection"]_

1. _Step 1_
2. _Step 2_

### 4.2 _[Subsection — e.g., "Test case structure"]_

1. _Step 1_
2. _Step 2_

---

## 5. Task breakdown

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | _Install/configure test framework (Google Test or Catch2)_ | 30 min | — |
| 2 | _Write test cases for `add()` method (normal, edge cases)_ | 1 hr | 1 |
| 3 | _Write test cases for `multiply()` method_ | 1 hr | 1 |
| 4 | _Write test cases for `divide()` method including div-by-zero_ | 1.5 hr | 1 |
| 5 | _Write test cases for `conjugate()` and `norm()` methods_ | 1 hr | 1 |
| 6 | _Run full test suite and measure code coverage_ | 30 min | 2,3,4,5 |
| 7 | _Update `current_state_report.md` with results_ | 15 min | 6 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| _Test framework incompatibility with build system_ | Medium | Medium | _Verify framework works with existing compiler before starting_ |
| _Division-by-zero handling undefined in current code_ | High | Low | _Test will expose behavior; plan PLAN-002 to add safeguard if needed_ |
| _Coverage target of 90% may be unreachable_ | Low | Low | _If unreachable, document why and adjust goal; re-evaluate in post-mortum_ |

---

## 7. Success criteria

- [ ] All goals from §2 verified
- [ ] All tests pass with 0 failures
- [ ] Code coverage report generated and >90% achieved
- [ ] `current_state_report.md` updated with results
- [ ] Test suite integrates with build system (Makefile or CMake)

---

## 8. References

- `AGENT_MD/spec.md` — Known Issues § (mentions missing test suite)
- `AGENT_MD/plan/current_state_report.md` — §6 Known Issues
- `include/myComplex.hpp` — API reference
- `src/myComplex.cpp` — Implementation details
