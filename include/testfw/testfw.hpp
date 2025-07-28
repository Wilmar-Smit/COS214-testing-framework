#ifndef TESTFW_HPP
#define TESTFW_HPP

#include <string>
#include <memory>
#include <vector>
#include <exception>
#include <iostream>
#include <functional>
#include <limits>
#include <cmath>
#include <map>

// --- Test Result Enum ---
enum class TestResult {
    PASSED,
    FAILED,
    ERROR
};

// --- Assertion Exception ---
class AssertionException : public std::exception {
public:
    AssertionException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

// --- TestCase Base Class ---
class TestCase {
public:
    virtual ~TestCase() = default;
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
    TestResult getResult() const { return result_; }
    std::string getErrorMessage() const { return error_message_; }
protected:
    virtual void setUp() {}
    virtual void run() = 0;
    virtual void tearDown() {}
private:
    TestResult result_ = TestResult::ERROR;
    std::string error_message_;
};

// --- Test Registrar ---
class TestRegistrar {
public:
    using Factory = std::function<std::unique_ptr<TestCase>()>;
    TestRegistrar(const std::string& name, Factory factory, std::vector<std::string> tags = {"unit"});
    static std::vector<std::tuple<std::string, Factory, std::vector<std::string>>>& registry();
};

// --- Test Registration Macros ---
#define TEST_CASE(name, ...) \
    class TestCase_##name : public TestCase { \
        void run() override; \
    }; \
    static TestRegistrar reg_##name(#name, []() { \
        return std::make_unique<TestCase_##name>(); \
    }, {__VA_ARGS__}); \
    void TestCase_##name::run()

#define TEST_FIXTURE(fixture, name, ...) \
    class TestCase_##fixture##_##name : public fixture { \
        void run() override; \
    }; \
    static TestRegistrar reg_##fixture##_##name(#fixture "." #name, []() { \
        return std::make_unique<TestCase_##fixture##_##name>(); \
    }, {__VA_ARGS__}); \
    void TestCase_##fixture##_##name::run()

// --- Minimal Assert Namespace ---
namespace Assert {
    template<typename T, typename U>
    void equals(const T& expected, const U& actual, const std::string& message = "") {
        if (!(expected == actual)) {
            throw AssertionException(message.empty() ? "Assert::equals failed" : message);
        }
    }
    template<typename T, typename U>
    void notEquals(const T& expected, const U& actual, const std::string& message = "") {
        if (expected == actual) {
            throw AssertionException(message.empty() ? "Assert::notEquals failed" : message);
        }
    }
    inline void isTrue(bool condition, const std::string& message = "") {
        if (!condition) {
            throw AssertionException(message.empty() ? "Assert::isTrue failed" : message);
        }
    }
    inline void isFalse(bool condition, const std::string& message = "") {
        if (condition) {
            throw AssertionException(message.empty() ? "Assert::isFalse failed" : message);
        }
    }
    inline void isNull(const void* ptr, const std::string& message = "") {
        if (ptr != nullptr) {
            throw AssertionException(message.empty() ? "Assert::isNull failed" : message);
        }
    }
    inline void isNotNull(const void* ptr, const std::string& message = "") {
        if (ptr == nullptr) {
            throw AssertionException(message.empty() ? "Assert::isNotNull failed" : message);
        }
    }
    template<typename T>
    void almostEquals(T expected, T actual, T epsilon = std::numeric_limits<T>::epsilon(), const std::string& message = "") {
        if (std::abs(expected - actual) > epsilon) {
            throw AssertionException(message.empty() ? "Assert::almostEquals failed" : message);
        }
    }
    inline void stringContains(const std::string& haystack, const std::string& needle, const std::string& message = "") {
        if (haystack.find(needle) == std::string::npos) {
            throw AssertionException(message.empty() ? "Assert::stringContains failed" : message);
        }
    }
    inline void stringStartsWith(const std::string& str, const std::string& prefix, const std::string& message = "") {
        if (str.rfind(prefix, 0) != 0) {
            throw AssertionException(message.empty() ? "Assert::stringStartsWith failed" : message);
        }
    }
    inline void stringEndsWith(const std::string& str, const std::string& suffix, const std::string& message = "") {
        if (str.size() < suffix.size() || str.compare(str.size() - suffix.size(), suffix.size(), suffix) != 0) {
            throw AssertionException(message.empty() ? "Assert::stringEndsWith failed" : message);
        }
    }
    template<typename ExceptionType, typename Callable>
    void throws(Callable&& callable, const std::string& message = "") {
        bool thrown = false;
        try {
            callable();
        } catch (const ExceptionType&) {
            thrown = true;
        } catch (...) {
            throw AssertionException(message.empty() ? "Assert::throws: Wrong exception type thrown" : message);
        }
        if (!thrown) {
            throw AssertionException(message.empty() ? "Assert::throws: Exception not thrown" : message);
        }
    }
    template<typename Callable>
    void doesNotThrow(Callable&& callable, const std::string& message = "") {
        try {
            callable();
        } catch (...) {
            throw AssertionException(message.empty() ? "Assert::doesNotThrow: Unexpected exception thrown" : message);
        }
    }
    template<typename Container>
    void isEmpty(const Container& container, const std::string& message = "") {
        if (!container.empty()) {
            throw AssertionException(message.empty() ? "Assert::isEmpty failed" : message);
        }
    }
    template<typename Container>
    void hasSize(const Container& container, size_t expected_size, const std::string& message = "") {
        if (container.size() != expected_size) {
            throw AssertionException(message.empty() ? "Assert::hasSize failed" : message);
        }
    }
    template<typename Container, typename Element>
    void contains(const Container& container, const Element& element, const std::string& message = "") {
        if (std::find(container.begin(), container.end(), element) == container.end()) {
            throw AssertionException(message.empty() ? "Assert::contains failed" : message);
        }
    }
    template<typename Container, typename Element>
    void doesNotContain(const Container& container, const Element& element, const std::string& message = "") {
        if (std::find(container.begin(), container.end(), element) != container.end()) {
            throw AssertionException(message.empty() ? "Assert::doesNotContain failed" : message);
        }
    }
    template<typename Base, typename Derived>
    void isInstanceOf(const Base* obj, const std::string& message = "") {
        if (dynamic_cast<const Derived*>(obj) == nullptr) {
            throw AssertionException(message.empty() ? "Assert::isInstanceOf failed" : message);
        }
    }
    template<typename Interface>
    void implementsInterface(const Interface* obj, const std::string& message = "") {
        if (obj == nullptr) {
            throw AssertionException(message.empty() ? "Assert::implementsInterface failed" : message);
        }
    }
    template<typename Base>
    void isPolymorphic(const Base* obj, const std::string& expected_type, const std::string& message = "") {
        if (typeid(*obj).name() != expected_type) {
            throw AssertionException(message.empty() ? "Assert::isPolymorphic failed" : message);
        }
    }
    template<typename T>
    void stateEquals(const T& object, const std::string& expected_state, const std::string& message = "") {
        if (object.getState() != expected_state) {
            throw AssertionException(message.empty() ? "Assert::stateEquals failed" : message);
        }
    }
}

// --- Memory Leak Detection Abstraction ---
class MemoryReport {
public:
    bool hasLeaks = false;
    std::string details;
};

class MemoryChecker {
public:
    virtual ~MemoryChecker() = default;
    virtual void startMonitoring() = 0;
    virtual void stopMonitoring() = 0;
    virtual MemoryReport generateReport() = 0;
};

#ifdef __APPLE__
#include <cstdlib>
class LeaksChecker : public MemoryChecker {
public:
    void startMonitoring() override {
        // macOS: enable MallocStackLogging for leaks tool
        setenv("MallocStackLogging", "1", 1);
    }
    void stopMonitoring() override {
        // No-op for now
    }
    MemoryReport generateReport() override {
        // Run leaks tool on current process
        MemoryReport report;
        char cmd[128];
        snprintf(cmd, sizeof(cmd), "leaks %d > leaks_report.txt 2>&1", getpid());
        int ret = system(cmd);
        report.hasLeaks = (ret != 0);
        report.details = "See leaks_report.txt";
        return report;
    }
};
#endif

#ifdef __linux__
class ValgrindChecker : public MemoryChecker {
public:
    void startMonitoring() override {
        // No-op: valgrind runs the whole process
    }
    void stopMonitoring() override {}
    MemoryReport generateReport() override {
        // In real use, valgrind would be run externally
        MemoryReport report;
        report.hasLeaks = false; // Placeholder
        report.details = "Valgrind output should be checked externally.";
        return report;
    }
};
#endif

class MemoryCheckerFactory {
public:
    static std::unique_ptr<MemoryChecker> create() {
#ifdef __APPLE__
        return std::make_unique<LeaksChecker>();
#elif defined(__linux__)
        return std::make_unique<ValgrindChecker>();
#else
        return nullptr;
#endif
    }
};

// --- Configuration System ---
struct ExecutionConfig {
    bool parallel = false;
    int timeout_seconds = 30;
    bool memory_check = true;
    std::string memory_tool = "auto";
    bool strict_mode = true;
    bool pattern_verification = true;
};

struct OutputConfig {
    std::string format = "console";
    bool verbose = true;
    std::string file = "test_results.json";
    bool include_performance = true;
    bool generate_report = true;
    std::string report_format = "markdown";
};

struct FilterConfig {
    std::vector<std::string> include_patterns = {"*"};
    std::vector<std::string> exclude_patterns;
    std::vector<std::string> tags = {"unit", "integration", "pattern"};
    std::vector<std::string> test_types = {"unit", "integration", "pattern_verification"};
};

struct MemoryConfig {
    std::vector<std::string> valgrind_options = {"--leak-check=full", "--show-leak-kinds=all"};
    std::vector<std::string> leaks_options = {"-atExit"};
    bool fail_on_leak = true;
};

struct Config {
    ExecutionConfig execution;
    OutputConfig output;
    FilterConfig filters;
    MemoryConfig memory;
};

class ConfigParser {
public:
    static Config parseFromFile(const std::string& filename);
    static Config parseFromJson(const std::string& json);
};

// --- TestRegistrar Implementation ---
inline std::vector<std::tuple<std::string, TestRegistrar::Factory, std::vector<std::string>>>& TestRegistrar::registry() {
    static std::vector<std::tuple<std::string, Factory, std::vector<std::string>>> reg;
    return reg;
}

inline TestRegistrar::TestRegistrar(const std::string& name, Factory factory, std::vector<std::string> tags) {
    registry().emplace_back(name, factory, std::move(tags));
}

// --- Command Pattern for CLI ---
class Command {
public:
    virtual ~Command() = default;
    virtual int execute(const std::vector<std::string>& args) = 0;
    virtual std::string getDescription() const = 0;
};

class RunTestsCommand : public Command {
public:
    int execute(const std::vector<std::string>& args) override {
        // Placeholder: run all registered tests
        auto& tests = TestRegistrar::registry();
        int failed = 0;
        for (const auto& [name, factory, _] : tests) {
            auto test = factory();
            test->execute();
            if (test->getResult() != TestResult::PASSED) ++failed;
        }
        return failed;
    }
    std::string getDescription() const override { return "Run all tests"; }
};

class ListTestsCommand : public Command {
public:
    int execute(const std::vector<std::string>&) override {
        auto& tests = TestRegistrar::registry();
        for (const auto& [name, _] : tests) {
            std::cout << name << std::endl;
        }
        return 0;
    }
    std::string getDescription() const override { return "List all available tests"; }
};

class HelpCommand : public Command {
public:
    int execute(const std::vector<std::string>&) override {
        std::cout << "Usage: testfw [OPTIONS] [PATTERN]\n";
        std::cout << "  -l, --list      List available tests\n";
        std::cout << "  -h, --help      Show this help message\n";
        std::cout << "  (Other options omitted for brevity)\n";
        return 0;
    }
    std::string getDescription() const override { return "Show help message"; }
};

// --- Output Strategy Pattern ---
class OutputStrategy {
public:
    virtual ~OutputStrategy() = default;
    virtual void begin() = 0;
    virtual void reportTest(const std::string& name, TestResult result, const std::string& message = "") = 0;
    virtual void end(int passed, int failed, int errors) = 0;
};

class ConsoleOutputStrategy : public OutputStrategy {
public:
    void begin() override {
        std::cout << "==== Test Results ====" << std::endl;
    }
    void reportTest(const std::string& name, TestResult result, const std::string& message = "") override {
        std::cout << "[TEST] " << name << ": ";
        switch (result) {
            case TestResult::PASSED:
                std::cout << "PASSED";
                break;
            case TestResult::FAILED:
                std::cout << "FAILED: " << message;
                break;
            case TestResult::ERROR:
                std::cout << "ERROR: " << message;
                break;
        }
        std::cout << std::endl;
    }
    void end(int passed, int failed, int errors) override {
        std::cout << "\nSummary: " << passed << " passed, " << failed << " failed, " << errors << " errors." << std::endl;
    }
};

class JsonOutputStrategy : public OutputStrategy {
public:
    void begin() override {
        std::cout << "[\n";
        first_ = true;
    }
    void reportTest(const std::string& name, TestResult result, const std::string& message = "") override {
        if (!first_) std::cout << ",\n";
        std::cout << "  {\"test\": \"" << name << "\", \"result\": \"";
        switch (result) {
            case TestResult::PASSED: std::cout << "PASSED"; break;
            case TestResult::FAILED: std::cout << "FAILED"; break;
            case TestResult::ERROR:  std::cout << "ERROR"; break;
        }
        std::cout << "\", \"message\": \"" << escapeJson(message) << "\"}";
        first_ = false;
    }
    void end(int passed, int failed, int errors) override {
        std::cout << "\n],\n{\"summary\": {\"passed\": " << passed << ", \"failed\": " << failed << ", \"errors\": " << errors << "}}\n";
    }
private:
    bool first_ = true;
    static std::string escapeJson(const std::string& s) {
        std::string out;
        for (char c : s) {
            switch (c) {
                case '\\': out += "\\\\"; break;
                case '"': out += "\\\""; break;
                case '\n': out += "\\n"; break;
                default: out += c;
            }
        }
        return out;
    }
};

// --- Observer Pattern for Test Reporting ---
class TestObserver {
public:
    virtual ~TestObserver() = default;
    virtual void onTestStart(const std::string& name) {}
    virtual void onTestFinish(const std::string& name, TestResult result, const std::string& message = "") {}
    virtual void onSuiteStart(const std::string& suiteName) {}
    virtual void onSuiteFinish(const std::string& suiteName) {}
};

class ConsoleTestObserver : public TestObserver {
public:
    void onTestStart(const std::string& name) override {
        std::cout << "[RUNNING] " << name << std::endl;
    }
    void onTestFinish(const std::string& name, TestResult result, const std::string& message = "") override {
        std::cout << "[RESULT]  " << name << ": ";
        switch (result) {
            case TestResult::PASSED:
                std::cout << "PASSED";
                break;
            case TestResult::FAILED:
                std::cout << "FAILED: " << message;
                break;
            case TestResult::ERROR:
                std::cout << "ERROR: " << message;
                break;
        }
        std::cout << std::endl;
    }
    void onSuiteStart(const std::string& suiteName) override {
        std::cout << "\n[SUITE]  " << suiteName << std::endl;
    }
    void onSuiteFinish(const std::string& suiteName) override {
        std::cout << "[END SUITE] " << suiteName << std::endl;
    }
};

// --- Composite Pattern for Test Suite Organization ---
class TestComponent {
public:
    virtual ~TestComponent() = default;
    virtual void execute() = 0;
    virtual std::string getName() const = 0;
    virtual void add(std::unique_ptr<TestComponent> /*child*/) {}
    virtual size_t count() const { return 1; }
};

class TestLeaf : public TestComponent {
public:
    TestLeaf(const std::string& name, TestRegistrar::Factory factory)
        : name_(name), factory_(factory) {}
    void execute() override {
        auto test = factory_();
        test->execute();
        // Could notify observers here
    }
    std::string getName() const override { return name_; }
private:
    std::string name_;
    TestRegistrar::Factory factory_;
};

class TestSuite : public TestComponent {
public:
    TestSuite(const std::string& name) : name_(name) {}
    void add(std::unique_ptr<TestComponent> child) override {
        children_.emplace_back(std::move(child));
    }
    void execute() override {
        for (auto& child : children_) {
            child->execute();
        }
    }
    std::string getName() const override { return name_; }
    size_t count() const override { return children_.size(); }
private:
    std::string name_;
    std::vector<std::unique_ptr<TestComponent>> children_;
};

// --- TestRunner: Central Engine ---
class TestRunner {
public:
    TestRunner(Config config,
               std::unique_ptr<OutputStrategy> output = std::make_unique<ConsoleOutputStrategy>(),
               std::vector<std::shared_ptr<TestObserver>> observers = {})
        : config_(std::move(config)),
          output_(std::move(output)),
          observers_(std::move(observers)) {}

    void addObserver(std::shared_ptr<TestObserver> obs) {
        observers_.push_back(obs);
    }

    void runAll() {
        output_->begin();
        int passed = 0, failed = 0, errors = 0;
        auto& tests = TestRegistrar::registry();
        for (const auto& [name, factory, tags] : tests) {
            for (auto& obs : observers_) obs->onTestStart(name);
            auto test = factory();
            if (config_.execution.memory_check) {
                auto checker = MemoryCheckerFactory::create();
                checker->startMonitoring();
                test->execute();
                checker->stopMonitoring();
                auto report = checker->generateReport();
                if (report.hasLeaks && config_.memory.fail_on_leak) {
                    for (auto& obs : observers_) obs->onTestFinish(name, TestResult::ERROR, "Memory leak detected: " + report.details);
                    output_->reportTest(name, TestResult::ERROR, "Memory leak detected: " + report.details);
                    ++errors;
                    continue;
                }
            } else {
                test->execute();
            }
            TestResult result = test->getResult();
            std::string msg = test->getErrorMessage();
            for (auto& obs : observers_) obs->onTestFinish(name, result, msg);
            output_->reportTest(name, result, msg);
            switch (result) {
                case TestResult::PASSED: ++passed; break;
                case TestResult::FAILED: ++failed; break;
                case TestResult::ERROR: ++errors; break;
            }
        }
        output_->end(passed, failed, errors);
    }

    void runWithTag(const std::string& tag) {
        output_->begin();
        int passed = 0, failed = 0, errors = 0;
        auto& tests = TestRegistrar::registry();
        for (const auto& [name, factory, tags] : tests) {
            if (!tag.empty()) {
                bool found = false;
                for (const auto& t : tags) {
                    if (t == tag) { found = true; break; }
                }
                if (!found) continue;
            }
            for (auto& obs : observers_) obs->onTestStart(name);
            auto test = factory();
            if (config_.execution.memory_check) {
                auto checker = MemoryCheckerFactory::create();
                checker->startMonitoring();
                test->execute();
                checker->stopMonitoring();
                auto report = checker->generateReport();
                if (report.hasLeaks && config_.memory.fail_on_leak) {
                    for (auto& obs : observers_) obs->onTestFinish(name, TestResult::ERROR, "Memory leak detected: " + report.details);
                    output_->reportTest(name, TestResult::ERROR, "Memory leak detected: " + report.details);
                    ++errors;
                    continue;
                }
            } else {
                test->execute();
            }
            TestResult result = test->getResult();
            std::string msg = test->getErrorMessage();
            for (auto& obs : observers_) obs->onTestFinish(name, result, msg);
            output_->reportTest(name, result, msg);
            switch (result) {
                case TestResult::PASSED: ++passed; break;
                case TestResult::FAILED: ++failed; break;
                case TestResult::ERROR: ++errors; break;
            }
        }
        output_->end(passed, failed, errors);
    }

private:
    Config config_;
    std::unique_ptr<OutputStrategy> output_;
    std::vector<std::shared_ptr<TestObserver>> observers_;
};

#endif // TESTFW_HPP
