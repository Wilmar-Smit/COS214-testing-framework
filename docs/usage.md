# C++ Design Pattern Tester Usage Guide

This guide explains how to use the C++ Design Pattern Tester framework for academic assignments, including setup, writing tests, running tests, and interpreting results. It covers both general usage and pattern-specific verification.

---

## 1. Getting Started

### 1.1. Prerequisites
- C++17 or newer
- g++ (Linux/macOS) or clang++ (macOS)
- CMake or Makefile (provided)

### 1.2. Project Structure
Your assignment should follow this structure:
```
assignment_project/
├── src/                  # Your implementation files
├── tests/                # Test files (unit, integration, pattern)
├── include/              # Framework headers (testfw/)
├── docs/                 # Documentation and reports
├── config/               # JSON config files
├── makefile or CMakeLists.txt
└── ...
```

---

## 2. Writing Tests

### 2.1. Basic Test Case
Use the `TEST_CASE` macro for simple tests:
```cpp
#include <testfw/testfw.hpp>

TEST_CASE("Addition Works") {
    int sum = 2 + 2;
    Assert::equals(4, sum);
}
```

### 2.2. Test Fixtures
For setup/teardown logic, use `TEST_FIXTURE`:
```cpp
class MyFixture : public TestCase {
protected:
    void setUp() override { /* ... */ }
    void tearDown() override { /* ... */ }
};

TEST_FIXTURE(MyFixture, "Test with Fixture") {
    // Test code using fixture
}
```

### 2.3. Pattern Verification Tests
Use the pattern tester headers for design pattern compliance:
```cpp
#include <testfw/patterns/strategy_tester.hpp>
#include "../src/MyStrategy.hpp"

TEST_CASE("Strategy Pattern - Compliance") {
    // Use the provided tester
    testfw::patterns::StrategyPatternTester::verify(/* ... */);
}
```
See [Pattern Verification Helpers](pattern_verification.md) for pattern-specific examples.

---

## 3. Running Tests

### 3.1. Build and Run
- Use `make test` or `cmake` targets to build and run all tests.
- Run `./test_runner` or the generated test binary.

### 3.2. CLI Options
```
./test_runner --help
  --tag unit           # Run only unit tests
  --tag integration    # Run only integration tests
  --tag pattern        # Run only pattern verification tests
  --pattern-check      # Run pattern compliance checks
  --memory-check       # Enable memory leak detection
  --output FILE        # Output results to file
  --format FORMAT      # Output format: console, json, xml, markdown
  --generate-report    # Generate a submission report
```

---

## 4. Memory Leak Detection
- Use `make test-memory` or `./test_runner --memory-check` to run tests with Valgrind (Linux) or Leaks (macOS).
- Reports are generated in `docs/`.

---

## 5. Pattern Verification
- Include the relevant pattern tester header from `include/testfw/patterns/`.
- Use the static methods to verify compliance (see [Pattern Verification Helpers](pattern_verification.md)).
- Example for Singleton:
```cpp
#include <testfw/patterns/singleton_tester.hpp>
#include "../src/MySingleton.hpp"

TEST_CASE("Singleton Pattern - Compliance") {
    testfw::patterns::SingletonPatternTester::verifyInstance<MySingleton>();
    testfw::patterns::SingletonPatternTester::verifyNoCopyOrMove<MySingleton>();
}
```

---

## 6. Demo: Full Pattern Test Example

### 6.1. Strategy Pattern Demo
```cpp
#include <testfw/testfw.hpp>
#include <testfw/patterns/strategy_tester.hpp>
#include "../src/PaymentStrategy.hpp"

TEST_CASE("Strategy Pattern - Runtime Switching") {
    PaymentProcessor processor;
    auto strat1 = std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25");
    auto strat2 = std::make_unique<PayPalPayment>("user@example.com");
    testfw::patterns::StrategyPatternTester::verify(
        processor,
        std::move(strat1),
        std::move(strat2),
        "Credit Card",
        "PayPal"
    );
}
```

### 6.2. Observer Pattern Demo
```cpp
#include <testfw/testfw.hpp>
#include <testfw/patterns/observer_tester.hpp>
#include "../src/WeatherStation.hpp"

TEST_CASE("Observer Pattern - Registration and Notification") {
    WeatherStation station;
    testfw::patterns::MockObserver obs;
    testfw::patterns::ObserverPatternTester::verifyRegistration(station, &obs);
    testfw::patterns::ObserverPatternTester::verifyNotification(station, &obs, "update");
    testfw::patterns::ObserverPatternTester::verifyDetachment(station, &obs);
}
```

---

## 7. Interpreting Results
- **PASS**: Test succeeded
- **FAIL**: Assertion failed (see message)
- **ERROR**: Exception thrown or memory leak detected
- **REPORTS**: See `docs/` for detailed reports and memory analysis

---

## 8. Advanced Features
- **Custom Assertions**: Extend `Assert` for assignment-specific checks
- **Test Tags**: Use tags to organize and filter tests
- **Configuration**: Edit `config/assignment_config.json` for assignment-specific settings
- **Grading Mode**: Enable grading breakdown and penalties for leaks

---

## 9. Troubleshooting
- Ensure all required files are present (see `make validate`)
- Check for memory leaks before submission
- Use verbose output (`--verbose`) for detailed feedback
- Consult [Pattern Verification Helpers](pattern_verification.md) for pattern-specific help

---

## 10. Further Reading
- [Pattern Verification Helpers](pattern_verification.md)
- [Design Document](../design.md)
- [README.md](../README.md)

---

For questions, contact your course instructor or teaching assistant.
