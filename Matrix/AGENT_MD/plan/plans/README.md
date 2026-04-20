# Plans Index

This directory contains implementation plans documenting planned work on the Matrix Linear Algebra Library.

Each plan file follows the naming convention: `PLAN-NNN_<slug>.md`

## Active Plans

| Plan | Status | Created | Priority | Description |
|------|--------|---------|----------|-------------|
| [PLAN-008](PLAN-008_cleanup_release.md) | Draft | 2026-04-21 | Low | Code cleanup, refactoring, release prep for v1.0 |
| [PLAN-007](PLAN-007_performance_optimization.md) | Draft | 2026-04-21 | Medium | Performance profiling and optimization |
| [PLAN-006](PLAN-006_doxygen_docs.md) | Draft | 2026-04-21 | Medium | Doxygen API documentation generation |

## Completed Plans

| Plan | Report | Completed | Priority | Description |
|------|--------|-----------|----------|-------------|
| [PLAN-005](PLAN-005_solver_validation.md) | [REPORT-005](../reports/REPORT-005_solver_validation.md) | 2026-04-21 | High | Solver validation with comprehensive test matrices |
| [PLAN-004](PLAN-004_cmake_build.md) | [REPORT-004](../reports/REPORT-004_cmake_build.md) | 2026-04-21 | High | CMake build system for multi-platform support |
| [PLAN-003](PLAN-003_operator_overloading.md) | [REPORT-003](../reports/REPORT-003_operator_overloading.md) | 2026-04-21 | Medium | Operator overloading for intuitive API |
| [PLAN-002](PLAN-002_error_handling.md) | [REPORT-002](../reports/REPORT-002_error_handling.md) | 2026-04-21 | High | Error handling and input validation |
| [PLAN-001](PLAN-001_matrix_unit_tests.md) | [REPORT-001](../reports/REPORT-001_matrix_unit_tests.md) | 2026-04-21 | High | Comprehensive unit test suite |

## Plan Execution Sequence & Dependencies

```
PLAN-001 (Unit Tests)
    ↓
PLAN-002 (Error Handling) ← depends on test results from PLAN-001
    ↓
PLAN-003 (Operators) — independent but benefits from PLAN-002
PLAN-004 (CMake) — independent build system work
PLAN-005 (Solver Validation) ← depends on tests from PLAN-001
    ↓
PLAN-006 (Doxygen Docs) — documentation, independent
PLAN-007 (Performance) — optimization, after testing complete
    ↓
PLAN-008 (Release) — final polish and v1.0 release
```

## Template Reference

To create a new plan, copy `PLAN-000_template.md` to a new file: `PLAN-009_<slug>.md`

For authoring guidelines, see [../rules.md](../rules.md).
