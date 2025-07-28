## 6. Academic Usage Examples

### 6.1 Testing a Strategy Pattern Assignment
```cpp
#include <testfw/testfw.hpp>
#include <testfw/patterns/strategy_tester.hpp>

// Your implementation from UML diagram
#include "../src/PaymentStrategy.hpp"

// Unit tests for individual strategy classes
TEST_CASE("CreditCard - Basic Functionality") {
    CreditCardPayment cc("1234-5678-9012-3456", "12/25");
    
    // Test successful payment
    bool result = cc.processPayment(100.0);
    Assert::isTrue(result, "Credit card payment should succeed for valid amount");
    
    // Test payment type identification
    Assert::equals(std::string("Credit Card"), cc.getPaymentType());
}

TEST_CASE("CreditCard - Edge Cases and Error Handling") {
    CreditCardPayment cc("1234-5678-9012-3456", "12/25");
    
    // Test negative amount (should your design handle this?)
    Assert::throws<std::invalid_argument>([&]() {
        cc.processPayment(-50.0);
    }, "Negative amounts should throw exception");
    
    // Test zero amount
    Assert::throws<std::invalid_argument>([&]() {
        cc.processPayment(0.0);
    }, "Zero amounts should throw exception");
}

// Pattern compliance tests
TEST_CASE("Strategy Pattern - Interface Compliance") {
    // Verify polymorphic behavior
    std::unique_ptr<PaymentStrategy> strategy1 = 
        std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25");
    std::unique_ptr<PaymentStrategy> strategy2 = 
        std::make_unique<PayPalPayment>("user@example.com");
    
    // Both should implement the same interface
    Assert::isNotNull(strategy1.get());
    Assert::isNotNull(strategy2.get());
    
    // Test polymorphic method calls
    Assert::doesNotThrow([&]() {
        strategy1->processPayment(100.0);
        strategy2->processPayment(100.0);
    }, "Polymorphic calls should work");
}

// Context class testing
class PaymentProcessorTest : public TestCase {
protected:
    void setUp() override {
        processor = std::make_unique<PaymentProcessor>();
    }
    
    void tearDown() override {
        processor.reset();
    }
    
    std::unique_ptr<PaymentProcessor> processor;
};

TEST_FIXTURE(PaymentProcessorTest, "Strategy Switching At Runtime") {
    // This tests the core Strategy pattern behavior
    
    // Set initial strategy
    auto ccStrategy = std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25");
    processor->setPaymentStrategy(std::move(ccStrategy));
    
    Assert::equals(std::string("Credit Card"), processor->getCurrentPaymentType());
    Assert::isTrue(processor->executePayment(100.0));
    
    // Switch strategy at runtime (key Strategy pattern feature)
    auto paypalStrategy = std::make_unique<PayPalPayment>("user@example.com");
    processor->setPaymentStrategy(std::move(paypalStrategy));
    
    Assert::equals(std::string("PayPal"), processor->getCurrentPaymentType());
    Assert::isTrue(processor->executePayment(100.0));
}

// Integration test demonstrating the complete pattern
TEST_CASE("Strategy Pattern - Complete Workflow Integration") {
    PaymentProcessor processor;
    
    // Test with multiple strategies in sequence
    std::vector<std::unique_ptr<PaymentStrategy>> strategies;
    strategies.push_back(std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25"));
    strategies.push_back(std::make_unique<PayPalPayment>("user@example.com"));
    
    std::vector<double> amounts = {50.0, 100.0, 25.75, 200.0};
    
    for (auto& strategy : strategies) {
        processor.setPaymentStrategy(std::move(strategy));
        
        for (double amount : amounts) {
            bool result = processor.executePayment(amount);
            Assert::isTrue(result, 
                "Payment should succeed for " + processor.getCurrentPaymentType() + 
                " with amount: " + std::to_string(amount));
        }
    }
}
```

### 6.2 Testing Observer Pattern Assignment
```cpp
#include <testfw/testfw.hpp>
#include <testfw/patterns/observer_tester.hpp>

// Observer pattern implementation tests
TEST_CASE("Observer - Subject Registration") {
    WeatherStation station;
    auto display1 = std::make_unique<TemperatureDisplay>();
    auto display2 = std::make_unique<HumidityDisplay>();
    
    // Test observer registration
    station.addObserver(display1.get());
    station.addObserver(display2.get());
    
    Assert::equals(2, station.getObserverCount());
}

TEST_CASE("Observer - Notification Mechanism") {
    WeatherStation station;
    TestObserver testObs; // Mock observer for testing
    
    station.addObserver(&testObs);# C++ Testing Framework Design Document

## 1. Project Overview

### 1.1 Purpose
A comprehensive, cross-platform C++ testing framework specifically designed for university coursework and design pattern assignments. The framework provides industry-standard testing capabilities with integrated memory leak detection, enabling students to thoroughly test their implementations and verify design pattern correctness.

### 1.2 Target Platforms
- Linux (primary)
- macOS (secondary)
- C++17/20 standard
- g++ compiler support

### 1.3 Core Objectives
- **Academic Focus**: Tailored for testing design pattern implementations and UML-based assignments
- **Pattern Verification**: Ensure correct implementation of design patterns (Strategy, Observer, Factory, etc.)
- **Code Quality**: Memory leak detection, exception handling verification, and robust error checking
- **Assignment Workflow**: Support the typical assignment cycle: implement classes → write tests → verify correctness → submit
- **Professional Practice**: Teach modern C++ testing methodologies and industry-standard practices

## 2. System Architecture

### 2.1 Design Patterns Used

#### 2.1.1 Command Pattern
- **Purpose**: CLI command handling and test execution
- **Implementation**: Abstract `Command` base class with concrete implementations for `RunTests`, `ListTests`, `GenerateReport`

#### 2.1.2 Strategy Pattern
- **Purpose**: Multiple assertion strategies, output formats, memory checkers
- **Implementation**: 
  - `AssertionStrategy` for different comparison types
  - `OutputStrategy` for console/JSON/XML output
  - `MemoryCheckStrategy` for Valgrind/Leaks integration

#### 2.1.3 Observer Pattern
- **Purpose**: Test result notification and real-time reporting
- **Implementation**: `TestObserver` interface with concrete observers for different output formats

#### 2.1.4 Factory Pattern
- **Purpose**: Creating test instances, assertions, and platform-specific components
- **Implementation**: `TestFactory`, `AssertionFactory`, `MemoryCheckerFactory`

#### 2.1.5 Composite Pattern
- **Purpose**: Test suite organization (tests containing sub-tests)
- **Implementation**: `TestComponent` base class with `Test` and `TestSuite` implementations

#### 2.1.6 Template Method Pattern
- **Purpose**: Test execution lifecycle (setup, run, teardown)
- **Implementation**: Abstract `TestCase` with template method `execute()`

### 2.2 High-Level Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   CLI Module    │────│  Core Engine    │────│ Platform Layer  │
└─────────────────┘    └─────────────────┘    └─────────────────┘
│                       │                       │
│                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Config Parser   │    │ Test Discovery  │    │ Memory Checkers │
└─────────────────┘    └─────────────────┘    └─────────────────┘
│                       │                       │
│                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ Report Engine   │    │ Assertion Lib   │    │ File System     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## 3. Core Components

### 3.1 Test Framework Core (`TestCore`)

#### 3.1.1 TestCase Base Class
```cpp
class TestCase {
public:
    virtual ~TestCase() = default;
    
    // Template Method Pattern
    void execute() {
        setUp();
        try {
            run();
            result_ = TestResult::PASSED;
        } catch (const AssertionException& e) {
            result_ = TestResult::FAILED;
            error_message_ = e.what();
        } catch (const std::exception& e) {
            result_ = TestResult::ERROR;
            error_message_ = e.what();
        }
        tearDown();
    }
    
protected:
    virtual void setUp() {}
    virtual void run() = 0;
    virtual void tearDown() {}
    
private:
    TestResult result_;
    std::string error_message_;
};
```

#### 3.1.2 Test Registration System
```cpp
// Macro-based registration for ease of use
#define TEST_CASE(name) \
    class TestCase_##name : public TestCase { \
        void run() override; \
    }; \
    static TestRegistrar reg_##name(#name, []() { \
        return std::make_unique<TestCase_##name>(); \
    }); \
    void TestCase_##name::run()

#define TEST_FIXTURE(fixture, name) \
    class TestCase_##fixture##_##name : public fixture { \
        void run() override; \
    }; \
    static TestRegistrar reg_##fixture##_##name(#fixture "." #name, []() { \
        return std::make_unique<TestCase_##fixture##_##name>(); \
    }); \
    void TestCase_##fixture##_##name::run()
```

### 3.2 Assertion Library

#### 3.2.1 Core Assertions for Academic Testing
```cpp
namespace Assert {
    // Basic comparisons
    template<typename T, typename U>
    void equals(const T& expected, const U& actual, const std::string& message = "");
    
    template<typename T, typename U>
    void notEquals(const T& expected, const U& actual, const std::string& message = "");
    
    void isTrue(bool condition, const std::string& message = "");
    void isFalse(bool condition, const std::string& message = "");
    void isNull(const void* ptr, const std::string& message = "");
    void isNotNull(const void* ptr, const std::string& message = "");
    
    // Floating-point comparisons (important for numerical assignments)
    template<typename T>
    void almostEquals(T expected, T actual, T epsilon = std::numeric_limits<T>::epsilon());
    
    // String assertions (common in assignments)
    void stringContains(const std::string& haystack, const std::string& needle);
    void stringStartsWith(const std::string& str, const std::string& prefix);
    void stringEndsWith(const std::string& str, const std::string& suffix);
    void stringMatches(const std::string& str, const std::string& pattern);
    
    // Exception assertions (testing error handling)
    template<typename ExceptionType, typename Callable>
    void throws(Callable&& callable, const std::string& message = "");
    
    template<typename Callable>
    void doesNotThrow(Callable&& callable, const std::string& message = "");
    
    // Container assertions (for data structure assignments)
    template<typename Container>
    void isEmpty(const Container& container);
    
    template<typename Container>
    void hasSize(const Container& container, size_t expected_size);
    
    template<typename Container, typename Element>
    void contains(const Container& container, const Element& element);
    
    template<typename Container, typename Element>
    void doesNotContain(const Container& container, const Element& element);
    
    // Design Pattern Specific Assertions
    template<typename Base, typename Derived>
    void isInstanceOf(const Base* obj, const std::string& message = "");
    
    template<typename Interface>
    void implementsInterface(const Interface* obj, const std::string& message = "");
    
    // Polymorphism verification
    template<typename Base>
    void isPolymorphic(const Base* obj, const std::string& expected_type);
    
    // State verification (for State pattern, etc.)
    template<typename T>
    void stateEquals(const T& object, const std::string& expected_state);
}
```

#### 3.2.2 Strategy Pattern for Assertions
```cpp
class AssertionStrategy {
public:
    virtual ~AssertionStrategy() = default;
    virtual bool evaluate() const = 0;
    virtual std::string getFailureMessage() const = 0;
};

template<typename T, typename U>
class EqualsAssertion : public AssertionStrategy {
    // Implementation details
};
```

### 3.3 Memory Leak Detection

#### 3.3.1 Platform Abstraction
```cpp
class MemoryChecker {
public:
    virtual ~MemoryChecker() = default;
    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;
    virtual MemoryReport generateReport() = 0;
};

class ValgrindChecker : public MemoryChecker {
    // Linux implementation using Valgrind
};

class LeaksChecker : public MemoryChecker {
    // macOS implementation using Leaks tool
};
```

#### 3.3.2 Memory Check Integration
```cpp
class TestRunner {
    void runWithMemoryCheck(TestCase* test) {
        auto checker = MemoryCheckerFactory::create();
        checker->startMonitoring();
        test->execute();
        checker->stopMonitoring();
        auto report = checker->generateReport();
        // Process memory report
    }
};
```

### 3.4 Configuration System

#### 3.4.1 JSON Configuration Schema for Academic Use
```json
{
    "framework": {
        "name": "cpp-test-framework",
        "version": "1.0.0",
        "mode": "academic"
    },
    "assignment": {
        "name": "Strategy Pattern Implementation",
        "course": "Design Patterns CS301",
        "due_date": "2025-08-15",
        "pattern_type": "strategy"
    },
    "execution": {
        "parallel": false,
        "timeout_seconds": 30,
        "memory_check": true,
        "memory_tool": "auto",
        "strict_mode": true,
        "pattern_verification": true
    },
    "output": {
        "format": "console",
        "verbose": true,
        "file": "test_results.json",
        "include_performance": true,
        "generate_report": true,
        "report_format": "markdown"
    },
    "filters": {
        "include_patterns": ["*"],
        "exclude_patterns": [],
        "tags": ["unit", "integration", "pattern"],
        "test_types": ["unit", "integration", "pattern_verification"]
    },
    "grading": {
        "enable_grading_mode": false,
        "weight_unit_tests": 0.4,
        "weight_integration_tests": 0.3,
        "weight_pattern_compliance": 0.3,
        "memory_leak_penalty": 0.1
    },
    "memory": {
        "valgrind_options": ["--leak-check=full", "--show-leak-kinds=all"],
        "leaks_options": ["-atExit"],
        "fail_on_leak": true
    }
}
```

#### 3.4.2 Configuration Parser
```cpp
class ConfigParser {
public:
    static Config parseFromFile(const std::string& filename);
    static Config parseFromJson(const std::string& json);
    
private:
    // JSON parsing implementation
};

struct Config {
    ExecutionConfig execution;
    OutputConfig output;
    FilterConfig filters;
    MemoryConfig memory;
};
```

### 3.5 Command Line Interface

#### 3.5.1 Command Pattern Implementation
```cpp
class Command {
public:
    virtual ~Command() = default;
    virtual int execute(const std::vector<std::string>& args) = 0;
    virtual std::string getDescription() const = 0;
};

class RunTestsCommand : public Command {
public:
    int execute(const std::vector<std::string>& args) override;
    std::string getDescription() const override { return "Run tests"; }
};

class ListTestsCommand : public Command {
    // Implementation
};

class HelpCommand : public Command {
    // Implementation
};
```

#### 3.5.2 CLI Options for Academic Use
```
Usage: testfw [OPTIONS] [PATTERN]

Options:
  -c, --config FILE         Configuration file (default: test_config.json)
  -f, --format FORMAT       Output format: console, json, xml, markdown (default: console)
  -v, --verbose             Verbose output with detailed feedback
  -l, --list                List available tests
  -t, --tag TAG             Run tests with specific tag (unit, integration, pattern)
  -m, --memory              Enable memory checking (required for submission)
  -p, --parallel            Run tests in parallel
  -o, --output FILE         Output file for results
  -r, --report              Generate submission report
  -s, --strict              Enable strict mode (fail on warnings)
  --pattern-check           Verify design pattern implementation
  --assignment-mode         Run in assignment validation mode
  --grade                   Show grading breakdown
  -h, --help               Show this help message

Academic Examples:
  testfw                    Run all tests for current assignment
  testfw -m -v -r          Run with memory check, verbose output, and generate report
  testfw --pattern-check   Verify design pattern is correctly implemented
  testfw -t unit           Run only unit tests
  testfw -t pattern        Run pattern verification tests
  testfw --assignment-mode Complete assignment validation suite
  testfw "Strategy*"       Run tests matching Strategy pattern
```

## 4. File Structure for Academic Assignments

```
assignment_project/
├── src/                          # Your implementation files
│   ├── [PatternName].hpp        # Header files from UML diagram
│   ├── [ConcreteClass1].cpp     # Implementation files
│   ├── [ConcreteClass2].cpp
│   └── main.cpp                 # Demonstration main function
├── tests/                       # Comprehensive test suite
│   ├── unit/                    # Unit tests for individual classes
│   │   ├── test_[class1].cpp
│   │   ├── test_[class2].cpp
│   │   └── test_[interface].cpp
│   ├── integration/             # Integration tests
│   │   ├── test_pattern_integration.cpp
│   │   └── test_workflow.cpp
│   ├── pattern/                 # Design pattern verification tests
│   │   ├── test_pattern_compliance.cpp
│   │   ├── test_polymorphism.cpp
│   │   └── test_runtime_behavior.cpp
│   └── fixtures/                # Test fixtures and utilities
│       ├── test_fixtures.hpp
│       └── mock_objects.cpp
├── include/
│   └── testfw/                  # Testing framework headers
│       ├── core/
│       ├── assertions/
│       ├── patterns/            # Pattern-specific testing utilities
│       │   ├── strategy_tester.hpp
│       │   ├── observer_tester.hpp
│       │   ├── factory_tester.hpp
│       │   └── singleton_tester.hpp
│       └── academic/            # Academic-specific features
│           ├── assignment_validator.hpp
│           ├── uml_compliance.hpp
│           └── grading_helper.hpp
├── docs/                        # Assignment documentation
│   ├── test_results.md
│   ├── pattern_verification.md
│   └── memory_analysis.md
├── config/
│   ├── assignment_config.json   # Assignment-specific configuration
│   ├── unit_test_config.json
│   └── grading_config.json
├── tools/
│   ├── generate_report.sh       # Generate submission report
│   ├── validate_assignment.sh   # Full assignment validation
│   └── check_pattern.sh         # Quick pattern verification
├── CMakeLists.txt
├── Makefile                     # Enhanced for academic workflow
├── README.md                    # Assignment documentation
└── SUBMISSION.md                # Pre-submission checklist
```

## 5. Build System Integration for Academic Workflow

### 5.1 Enhanced Makefile for Assignment Testing
```makefile
# Assignment-specific variables
ASSIGNMENT_NAME ?= strategy_pattern
PATTERN_TYPE ?= strategy
DUE_DATE ?= $(shell date -d "+7 days" +%Y-%m-%d)

# Detect platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    MEMORY_TOOL = valgrind
    MEMORY_FLAGS = --leak-check=full --show-leak-kinds=all --error-exitcode=1
    MEMORY_REPORT = --xml=yes --xml-file=memory_report.xml
endif
ifeq ($(UNAME_S),Darwin)
    MEMORY_TOOL = leaks
    MEMORY_FLAGS = -atExit
    MEMORY_REPORT = > memory_report.txt
endif

# Compiler settings for academic rigor
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g -O0
STRICT_FLAGS = -Werror -Wconversion -Wsign-conversion -Wunused
INCLUDES = -Iinclude -Isrc
LIBS = -pthread

# Source files
SRCDIR = src
TESTDIR = tests
MAIN_SOURCES = $(filter-out $(SRCDIR)/main.cpp, $(wildcard $(SRCDIR)/*.cpp))
TEST_SOURCES = $(shell find $(TESTDIR) -name "*.cpp")
MAIN_OBJECTS = $(MAIN_SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

.PHONY: all build test test-memory test-unit test-integration test-pattern
.PHONY: assignment-check submission-ready validate clean install help

all: help

# Build the main demonstration program
build: $(MAIN_OBJECTS) $(SRCDIR)/main.o
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $(ASSIGNMENT_NAME) $(LIBS)
	@echo "✓ Built demonstration program: ./$(ASSIGNMENT_NAME)"

# Build and run all tests
test: test-build
	@echo "Running complete test suite for $(ASSIGNMENT_NAME)..."
	./test_runner --assignment-mode
	@echo "✓ All tests completed"

# Build test executable
test-build: $(MAIN_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o test_runner $(LIBS)

# Run tests with strict memory checking (required for submission)
test-memory: test-build
	@echo "Running memory leak detection..."
ifeq ($(UNAME_S),Linux)
	$(MEMORY_TOOL) $(MEMORY_FLAGS) $(MEMORY_REPORT) ./test_runner --memory-check
endif
ifeq ($(UNAME_S),Darwin)
	MallocStackLogging=1 ./test_runner --memory-check && $(MEMORY_TOOL) $(MEMORY_FLAGS) ./test_runner $(MEMORY_REPORT)
endif
	@echo "✓ Memory check completed"

# Run only unit tests
test-unit: test-build
	@echo "Running unit tests..."
	./test_runner --tag unit -v
	@echo "✓ Unit tests completed"

# Run integration tests
test-integration: test-build
	@echo "Running integration tests..."
	./test_runner --tag integration -v
	@echo "✓ Integration tests completed"

# Verify design pattern implementation
test-pattern: test-build
	@echo "Verifying $(PATTERN_TYPE) pattern implementation..."
	./test_runner --pattern-check --tag pattern -v
	@echo "✓ Pattern verification completed"

# Complete assignment validation (what you run before submission)
assignment-check: clean test-memory test-pattern
	@echo "Running complete assignment validation..."
	./test_runner --assignment-mode --generate-report
	@echo "✓ Assignment validation completed"
	@echo "Check generated reports in docs/ directory"

# Check if assignment is ready for submission
submission-ready: assignment-check
	@echo "Checking submission readiness..."
	@./tools/validate_assignment.sh $(ASSIGNMENT_NAME) $(PATTERN_TYPE)
	@echo "✓ Submission validation completed"

# Validate against common requirements
validate: test-build
	@echo "Running validation checks..."
	# Check for required files
	@test -f src/main.cpp || (echo "❌ Missing main.cpp" && exit 1)
	@test -d tests/ || (echo "❌ Missing tests directory" && exit 1)
	# Check for memory leaks
	@$(MAKE) test-memory > /dev/null 2>&1 || (echo "❌ Memory leaks detected" && exit 1)
	# Check pattern implementation
	@$(MAKE) test-pattern > /dev/null 2>&1 || (echo "❌ Pattern implementation issues" && exit 1)
	@echo "✓ All validation checks passed"

# Generate comprehensive report for submission
report: assignment-check
	@echo "Generating submission report..."
	@./tools/generate_report.sh $(ASSIGNMENT_NAME) $(PATTERN_TYPE) $(DUE_DATE)
	@echo "✓ Report generated: docs/submission_report.md"

# Build with strict compiler flags (for final validation)
strict: CXXFLAGS += $(STRICT_FLAGS)
strict: clean test

# Install framework for system-wide use
install:
	sudo cp test_runner /usr/local/bin/testfw
	sudo cp -r include/testfw /usr/local/include/
	@echo "✓ Testing framework installed system-wide"

# Clean all build artifacts
clean:
	rm -f $(MAIN_OBJECTS) $(TEST_OBJECTS) $(ASSIGNMENT_NAME) test_runner
	rm -f *.xml *.txt docs/test_results.* docs/memory_report.*
	@echo "✓ Cleaned build artifacts"

# Show help with academic-focused commands
help:
	@echo "Academic C++ Testing Framework - Makefile Help"
	@echo "=============================================="
	@echo ""
	@echo "Assignment Development:"
	@echo "  build              Build the main demonstration program"
	@echo "  test               Run all tests (unit + integration + pattern)"
	@echo "  test-unit          Run only unit tests"
	@echo "  test-integration   Run only integration tests"
	@echo "  test-pattern       Verify design pattern implementation"
	@echo ""
	@echo "Quality Assurance:"
	@echo "  test-memory        Run tests with memory leak detection"
	@echo "  strict             Build and test with strict compiler flags"
	@echo "  validate           Run comprehensive validation checks"
	@echo ""
	@echo "Submission Preparation:"
	@echo "  assignment-check   Complete pre-submission validation"
	@echo "  submission-ready   Final submission readiness check"
	@echo "  report             Generate comprehensive submission report"
	@echo ""
	@echo "Utilities:"
	@echo "  clean              Remove all build artifacts"
	@echo "  install            Install framework system-wide"
	@echo "  help               Show this help message"
	@echo ""
	@echo "Example workflow:"
	@echo "  make build && ./$(ASSIGNMENT_NAME)    # Test your main program"
	@echo "  make test-unit                        # Develop with unit tests"
	@echo "  make assignment-check                 # Pre-submission validation"
	@echo "  make submission-ready                 # Final check before submission"

# Individual file compilation with dependency tracking
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include dependency files
-include $(MAIN_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)
```

### 5.2 CMake Integration
```cmake
cmake_minimum_required(VERSION 3.10)
project(CppTestFramework VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(APPLE)
    set(MEMORY_TOOL "leaks")
elseif(UNIX)
    set(MEMORY_TOOL "valgrind")
endif()

# Add custom target for memory testing
add_custom_target(test-memory
    COMMAND ${CMAKE_COMMAND} -E env "MEMORY_TOOL=${MEMORY_TOOL}" $<TARGET_FILE:testfw>
    DEPENDS testfw
)
```

## 6. Usage Examples

### 6.1 Basic Test
```cpp
#include <testfw/testfw.hpp>

TEST_CASE("Basic Math Operations") {
    int result = 2 + 2;
    Assert::equals(4, result);
    Assert::notEquals(5, result);
}

TEST_CASE("String Operations") {
    std::string str = "Hello, World!";
    Assert::stringContains(str, "World");
    Assert::stringStartsWith(str, "Hello");
    Assert::hasSize(str, 13);
}
```

### 6.2 Test Fixtures
```cpp
class DatabaseTest : public TestCase {
protected:
    void setUp() override {
        db = std::make_unique<Database>("test.db");
        db->connect();
    }
    
    void tearDown() override {
        db->disconnect();
        db.reset();
    }
    
    std::unique_ptr<Database> db;
};

TEST_FIXTURE(DatabaseTest, "Insert Record") {
    Record record{"John", "Doe", 30};
    bool success = db->insert(record);
    Assert::isTrue(success);
    Assert::equals(1, db->count());
}
```

### 6.3 Exception Testing
```cpp
TEST_CASE("Exception Handling") {
    std::vector<int> vec;
    
    Assert::throws<std::out_of_range>([&]() {
        vec.at(10);
    });
    
    Assert::doesNotThrow([&]() {
        vec.push_back(42);
    });
}
```

## 7. Integration Points

### 7.1 Continuous Integration
- JUnit XML output for Jenkins/GitLab CI
- Exit codes for build system integration
- TAP output for additional compatibility

### 7.2 IDE Integration
- Test discovery for modern IDEs
- Debug information preservation
- Structured output for parsing

### 7.3 External Tools
- Valgrind integration on Linux
- Leaks tool integration on macOS
- Coverage tools (gcov, llvm-cov)

## 8. Implementation Phases

### Phase 1: Core Framework (Week 1-2)
- Basic test case structure
- Test registration system
- Simple assertions library
- Console output

### Phase 2: Advanced Features (Week 3-4)
- Memory leak detection
- JSON configuration
- CLI interface
- Multiple output formats

### Phase 3: Platform Integration (Week 5-6)
- Cross-platform build system
- Memory checker abstraction
- Performance optimizations
- Documentation

### Phase 4: Polish & Testing (Week 7-8)
- Self-testing (dogfooding)
- Error handling improvements
- User experience refinements
- Final documentation

## 9. Success Metrics

### 9.1 Functionality
- ✅ Comprehensive assertion library (20+ assertion types)
- ✅ Cross-platform memory leak detection
- ✅ Flexible configuration system
- ✅ Multiple output formats

### 9.2 Usability
- ✅ Simple test definition macros
- ✅ Automatic test discovery
- ✅ Clear error messages
- ✅ Comprehensive CLI

### 9.3 Integration
- ✅ Makefile integration
- ✅ CI/CD compatibility
- ✅ IDE-friendly output
- ✅ Platform-specific optimizations

This design provides a solid foundation for a professional-grade C++ testing framework that demonstrates multiple design patterns while delivering maximum functionality and usability.