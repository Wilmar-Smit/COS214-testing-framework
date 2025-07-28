// demo_commands.cpp
// Demonstrates usage of all test framework commands and options
#include <testfw/testfw.hpp>
#include <testfw/patterns/strategy_tester.hpp>
#include "../array.h"

// Unit test example
testfw::TestCase unit_test_case("Unit Test Example", []() {
    // ...unit test logic...
});

// Integration test example
testfw::TestCase integration_test_case("Integration Test Example", []() {
    // ...integration test logic...
});

// Pattern verification test example
testfw::TestCase pattern_test_case("Pattern Test Example", []() {
    testfw::patterns::StrategyPatternTester::verify(/* ... */);
});

int main(int argc, char** argv) {
    // Register tests with tags
    testfw::register_test(unit_test_case, {"unit"});
    testfw::register_test(integration_test_case, {"integration"});
    testfw::register_test(pattern_test_case, {"pattern"});

    // Run the test framework with CLI options
    // Example usage:
    //   ./test_runner --tag unit
    //   ./test_runner --tag integration
    //   ./test_runner --tag pattern
    //   ./test_runner --pattern-check
    //   ./test_runner --memory-check
    //   ./test_runner --output results.txt
    //   ./test_runner --format json
    //   ./test_runner --generate-report
    return testfw::run(argc, argv);
}

