# TODO List: C++ Academic Testing Framework

## 1. Pattern-Specific Test Helpers
- Implement helpers for Strategy, Observer, Factory, Singleton, etc. in `include/testfw/patterns/`
- Document usage in `docs/pattern_verification.md`

## 2. Tag-Based Test Filtering
- Allow running tests by tag (unit, integration, pattern) via CLI and config
- Integrate with TestRunner and CLI

## 3. Additional Output Formats
- Implement JSON, Markdown, and XML output strategies
- Add CLI/config support for selecting output format

## 4. Configuration System
- Complete ConfigParser to support JSON config files
- Validate config schema and provide error messages

## 5. Pattern Compliance Verification
- Add utilities for verifying design pattern implementation (e.g., interface checks, runtime switching)
- Integrate with assignment validation workflow

## 6. Memory Analysis Reporting
- Improve memory leak report parsing and output
- Document workflow in `docs/memory_analysis.md`

## 7. Grading and Assignment Validation
- Implement grading breakdown and assignment validation mode
- Add CLI options and reporting

## 8. Documentation
- Expand `docs/usage.md` with more academic examples
- Add `docs/pattern_verification.md`, `docs/memory_analysis.md`, `docs/configuration.md`
- Add Doxygen-style comments to public APIs

## 9. Build System Enhancements
- Add/expand Makefile and CMake integration for academic workflow
- Add scripts for report generation and assignment validation

## 10. IDE and CI Integration
- Add JUnit/TAP output for CI
- Document IDE integration steps

