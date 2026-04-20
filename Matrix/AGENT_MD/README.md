# AGENT_MD — AI-Assisted Software Development Framework

> Copy this entire folder into the root of your project.
> Then customise the files for your project.

## What This Is

A structured documentation framework that helps AI coding agents (Claude, Cursor, Gemini, etc.) develop software with you in a disciplined, traceable way. It provides:

- A **master specification** (`spec.md`) — the single source of truth for what the project is and where it's going
- A **plan/report lifecycle** — numbered plans (what to do) and reports (what was done) with matching IDs
- A **current state report** — a living snapshot of the real codebase, updated after major work
- **Authoring rules** — consistent formatting that any AI agent can follow

## Folder Structure

```
AGENT_MD/
├── README.md                         # This file — usage guide
├── spec.md                           # Master project specification
├── plan/
│   ├── current_state_report.md       # Living project state snapshot
│   ├── rules.md                      # Authoring conventions for plans & reports
│   ├── plans/
│   │   └── PLAN-000_template.md      # Template — copy to start a new plan
│   └── reports/
│       ├── README.md                 # Report index
│       └── REPORT-000_template.md    # Template — copy when closing a plan
```

> **Serial number `000` is reserved for templates.** Real plans and reports start at `001`.

## Setup

1. **Fill in `spec.md`** with your project overview, goals, and success criteria.
2. **Update `plan/current_state_report.md`** with an initial audit of your codebase.
3. **Use `plan/rules.md`** as a reference when creating new plans and reports.
4. When starting work, **copy `PLAN-000_template.md`** to `PLAN-001_<slug>.md` and fill in your plan.
5. When work is complete, **copy `REPORT-000_template.md`** to `REPORT-001_<slug>.md` and document results.

## How AI Agents Use This

When collaborating with an AI assistant on features or refactoring:

1. **Before starting** — the AI reads `spec.md` and `current_state_report.md` for context.
2. **During planning** — the AI proposes a plan file (or you provide one) that breaks work into tasks.
3. **During implementation** — the AI refers to `plan/rules.md` to keep documentation consistent.
4. **After completion** — the AI generates a report matching the plan, and updates `current_state_report.md`.

## Key Concepts

| Term | Meaning |
|------|---------|
| **spec.md** | Living source of truth: what the project is, goals, constraints, current focus |
| **Current State Report** | Snapshot of the real codebase: inventory, config, tests, known issues |
| **PLAN-NNN** | Forward-looking: "here's what we will do" — broken into small, trackable tasks |
| **REPORT-NNN** | Backward-looking: "here's what we did" — results, changes, lessons learned |
| **rules.md** | Format & style guide for all plans and reports — ensures consistency across AI sessions |

---

For detailed guidance on writing plans and reports, see [plan/rules.md](plan/rules.md).
