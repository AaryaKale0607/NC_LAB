# Plan & Report Authoring Rules

> **Purpose:** This file defines the conventions for all plan and report documents
> stored under `plan/`. It is designed to be referenced in LLM/agent prompts so
> that any AI assistant producing plans or reports follows a consistent format.
>
> **Usage in prompts:** Include the directive
> `Follow the rules in AGENT_MD/plan/rules.md when writing plans or reports.`

---

## 1. Directory Layout

```
plan/
├── rules.md                          # THIS FILE — authoring conventions
├── current_state_report.md           # Latest project state snapshot
├── plans/                            # One file per plan
│   └── PLAN-NNN_<slug>.md
└── reports/                          # One file per implementation report
    └── REPORT-NNN_<slug>.md
```

- **plans/** stores *forward-looking* documents (what will be done).
- **reports/** stores *backward-looking* documents (what was done, results, learnings).
- A report's NNN **must match** the plan it implements (e.g., `PLAN-001` → `REPORT-001`).
- `current_state_report.md` is a living document updated whenever a major audit is performed.

---

## 2. Naming Conventions

| Item | Pattern | Example |
|------|---------|---------|
| Plan file | `PLAN-NNN_<short-slug>.md` | `PLAN-001_matrix_tests.md` |
| Report file | `REPORT-NNN_<short-slug>.md` | `REPORT-001_matrix_tests.md` |
| Slug | lowercase, underscores, ≤ 5 words | `add_unit_test_suite` |
| NNN | Zero-padded 3-digit serial | `001`, `002`, … |

---

## 3. Plan Document Template

Every plan **must** contain these sections in order:

```markdown
# PLAN-NNN: <Title>

**Created:** YYYY-MM-DD
**Status:** Draft | Approved | In-Progress | Completed | Abandoned
**Addresses:** <one-line description of the gap/need this plan targets>

---

## 1. Context & motivation
Why this plan exists. Link to the current state report or prior reports
that surfaced the need.

## 2. Goals
Bulleted list of measurable outcomes.
Each goal should be verifiable (pass/fail, metric threshold, etc.).

## 3. Non-goals
What is explicitly out of scope.

## 4. Approach
Detailed description of the technical approach.
Include architecture decisions, trade-offs, and alternatives considered.

## 5. Task breakdown
Numbered, actionable tasks. Each task should be small enough to complete
in a single working session (< 4 hours). Use this format:

| # | Task | Est. | Depends on |
|---|------|------|------------|
| 1 | … | 30 min | — |
| 2 | … | 2 hr | 1 |

## 6. Risks & mitigations
| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| … | Low/Med/High | Low/Med/High | … |

## 7. Success criteria
How we know the plan is fully implemented.
Reference specific goals from §2.

## 8. References
Links to docs, files, external resources.
```

---

## 4. Report Document Template

Every report **must** contain these sections in order:

```markdown
# REPORT-NNN: <Title>

**Plan:** PLAN-NNN
**Completed:** YYYY-MM-DD
**Author:** <human or AI agent identifier>

---

## 1. Summary
One paragraph: what was implemented, key outcomes.

## 2. Goals vs. actuals
Table mapping each goal from the plan to its actual outcome.

| Goal (from plan) | Outcome | Evidence |
|-------------------|---------|----------|
| … | ✅ Met / ⚠️ Partial / ❌ Not met | link or description |

## 3. Changes made
List every file created, modified, or deleted.
Group by logical unit (e.g., "Matrix operations", "Solver tests").

### 3.1 <Logical unit>
- `path/to/file.cpp` — description of change
- …

## 4. Testing & validation
How correctness was verified. Include command outputs, test results,
or manual verification steps.

## 5. Known issues & follow-ups
Anything left unresolved. Link to follow-up plan if one exists.

## 6. Metrics (if applicable)
Before/after test counts, code coverage, performance numbers, etc.

## 7. Lessons learned
What went well, what was harder than expected, what to do differently.
```

---

## 5. Writing Style Rules

These apply to **both** plans and reports:

1. **Be concrete, not vague.** Write "Add unit tests for Gaussian elimination with pivoting in `tests/test_gaussian.cpp`" — not "Test the solver."

2. **Reference files by relative path** from project root (e.g., `src/matrix.cpp`, `include/matrix.hpp`).

3. **Use present tense** in plans ("We add…", "We create…"), **past tense** in reports ("We added…", "We created…").

4. **Include code snippets** when they clarify intent, but keep them short (< 20 lines). Use fenced blocks with language tags.

5. **No orphan acronyms.** First use must expand: "Least Squares (LS)", "LU factorization (Lower-Upper)".

6. **Dates** use ISO 8601: `YYYY-MM-DD`.

7. **Use clear section headings** — avoid vague names like "Stuff" or "Other Things".

---

## 6. Quality Checklist for Plans

Before finalizing a plan, verify:

- [ ] Title is descriptive (not "Work on stuff")
- [ ] Context explains *why* we're doing this (links to current state report or prior reports)
- [ ] Goals are measurable and verifiable
- [ ] Non-goals explicitly state what's out of scope
- [ ] Approach is concrete (not "do better things")
- [ ] Tasks are small enough to fit in < 4 hour sessions
- [ ] Dependencies between tasks are clear
- [ ] Risks are identified with mitigations
- [ ] Success criteria are testable

---

## 7. Quality Checklist for Reports

Before finalizing a report, verify:

- [ ] Summary is one paragraph and captures key outcomes
- [ ] Goals from plan are addressed (all rows filled in §2)
- [ ] All changed files are listed (creation, modification, deletion)
- [ ] Testing approach is documented with concrete evidence (test output, commands)
- [ ] Metrics show before/after comparison
- [ ] Lessons learned are honest and specific
- [ ] Any unresolved issues or follow-ups are listed

---

## 8. Examples

### Good Plan Title
✅ `PLAN-001_matrix_unit_test_suite`

### Bad Plan Title
❌ `PLAN-001_testing`

### Good Goal
✅ "Achieve >90% branch coverage on all Matrix class operations as measured by gcov"

### Bad Goal
❌ "Improve test coverage"

### Good Task
✅ "Write 8 unit test cases for Matrix multiplication covering normal, edge cases, and dimension mismatches"

### Bad Task
❌ "Test matrix operations" (too vague, no timeframe)
