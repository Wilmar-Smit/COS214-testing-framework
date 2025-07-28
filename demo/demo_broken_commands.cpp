// demo_broken_commands.cpp
// Demonstrates failed tests in the test framework
#include <testfw/testfw.hpp>
#include <testfw/patterns/strategy_tester.hpp>
#include "../array.h"

// Failing unit test example
testfw::TestCase failing_unit_test("Failing Unit Test", []() {
    // Intentionally fail
    testfw::assert_equal(1, 2, "1 is not equal to 2");
});

// Failing integration test example
testfw::TestCase failing_integration_test("Failing Integration Test", []() {
    // Intentionally fail
    testfw::assert_true(false, "This integration test always fails");
});

// Failing pattern verification test example
testfw::TestCase failing_pattern_test("Failing Pattern Test", []() {
    // Intentionally fail pattern verification
    testfw::patterns::StrategyPatternTester::verify(nullptr); // Pass invalid argument
});

int main(int argc, char** argv) {
    // Register failing tests with tags
    testfw::register_test(failing_unit_test, {"unit"});
    testfw::register_test(failing_integration_test, {"integration"});
    testfw::register_test(failing_pattern_test, {"pattern"});

    // Run the test framework with CLI options
    // Example usage:
    //   ./test_runner_broken --tag unit
    //   ./test_runner_broken --tag integration
    //   ./test_runner_broken --tag pattern
    return testfw::run(argc, argv);
}

