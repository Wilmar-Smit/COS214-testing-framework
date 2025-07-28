# C++ Academic Testing Framework

A cross-platform C++ testing framework designed for university coursework, design pattern assignments, and academic rigor. Supports memory leak detection, pattern verification, and robust assertions.

## Features
- Simple test macros: `TEST_CASE`, `TEST_FIXTURE`
- Pattern verification helpers (Strategy, Observer, etc.)
- Memory leak detection (Valgrind/Leaks)
- Flexible output: console, JSON, Markdown (extensible)
- CLI for academic workflows
- Automatic test discovery and registration
- Comprehensive assertion library

## Quick Start
1. **Write a test:**
   ```cpp
   #include <testfw/testfw.hpp>

   TEST_CASE("Addition Works") {
       Assert::equals(4, 2 + 2);
   }
   ```
2. **Build and run:**
   ```sh
   make test
   ./test_runner
   ```
3. **Use CLI options:**
   ```sh
   ./test_runner --list
   ./test_runner --tag unit
   ./test_runner --memory
   ```

## Example: Strategy Pattern Assignment
```cpp
#include <testfw/testfw.hpp>
#include <testfw/patterns/strategy_tester.hpp>
#include "../src/PaymentStrategy.hpp"

TEST_CASE("CreditCard - Basic Functionality") {
    CreditCardPayment cc("1234-5678-9012-3456", "12/25");
    Assert::isTrue(cc.processPayment(100.0));
    Assert::equals(std::string("Credit Card"), cc.getPaymentType());
}
```

## Directory Structure
```
assignment_project/
  demo/
    demo_commands.cpp         # Demonstrates all commands/options (all tests pass)
    demo_broken_commands.cpp  # Demonstrates failed tests (intentional failures)
  include/
    testfw/                  # Testing framework headers
      patterns/              # Design pattern testers
  array.cpp, array.h         # Example code under test
  main.cpp                   # Example main/test runner
  makefile                   # Build instructions
  testfw.hpp                 # Main test framework header
  docs/                      # Documentation (usage, pattern verification, etc.)

## Demo: All Commands & Options
The `demo/demo_commands.cpp` file demonstrates how to use every major command and CLI option. It includes unit, integration, and pattern verification tests, and shows how to:

- Register tests with tags
- Use CLI options like `--tag`, `--pattern-check`, `--memory-check`, `--output`, `--format`, and `--generate-report`

**Build and run:**
```sh
make demo_commands
./demo/demo_commands --tag unit
./demo/demo_commands --pattern-check
./demo/demo_commands --output results.txt --format json
```

## Demo: Failed Tests
The `demo/demo_broken_commands.cpp` file contains tests that are intentionally designed to fail. Use this to see how the framework reports errors and failed assertions.

**Build and run:**
```sh
make demo_broken_commands
./demo/demo_broken_commands
```

## Interpreting Output
- Passing tests will be shown in green (or as `PASS` in text/JSON output).
- Failing tests will be shown in red (or as `FAIL`), with error messages and assertion details.
- Use `--output` and `--format` to generate machine-readable reports.

## Documentation
- See `docs/usage.md` for all CLI options and advanced usage.
- See `docs/pattern_verification.md` for design pattern test helpers.

## Academic Workflow
- Write implementation from UML
- Write unit, integration, and pattern tests
- Run `make test` and `make test-memory`
- Use `make assignment-check` before submission

## Contributing
See [docs/contributing.md](docs/contributing.md) for guidelines.

## License
MIT
