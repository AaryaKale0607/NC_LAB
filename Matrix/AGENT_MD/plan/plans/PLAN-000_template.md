# PLAN-000_template.md — Use as Template for New Plans

**Created:** YYYY-MM-DD
**Status:** Draft
**Addresses:** _[One-line description of the gap or need this plan targets.]_

---

## 1. Context & motivation

_[Why this plan exists. Link to the current state report or prior reports that surfaced the need.]_

Example: The Current State Report identified that the Matrix library lacks comprehensive unit tests. All solver implementations are verified only by code inspection and manual testing, not by automated test suite.

---

## 2. Goals

- **G1:** _[Measurable outcome — e.g., "Create unit test suite with >90% branch coverage for all Matrix operations."]_
- **G2:** _[Measurable outcome — e.g., "All solver implementations pass edge case tests."]_
- **G3:** _[Measurable outcome — e.g., "Test suite integrates with CMake build system."]_

---

## 3. Non-goals

- _[What's explicitly out of scope — e.g., "Performance optimization deferred to PLAN-X."]_
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
| 1 | _..._ | 30 min | — |
| 2 | _..._ | 2 hr | 1 |

---

## 6. Risks & mitigations

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| _..._ | Low/Med/High | Low/Med/High | _..._ |

---

## 7. Success criteria

- [ ] All goals from §2 verified
- [ ] All tests green
- [ ] Coverage metrics documented
- [ ] No regressions in existing functionality

---

## 8. References

- `AGENT_MD/spec.md` — Project specification
- `AGENT_MD/plan/current_state_report.md` — Current state audit
