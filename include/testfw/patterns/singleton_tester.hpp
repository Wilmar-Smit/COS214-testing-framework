#ifndef TESTFW_PATTERNS_SINGLETON_TESTER_HPP
#define TESTFW_PATTERNS_SINGLETON_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class SingletonPatternTester {
public:
    // Verifies that the singleton returns the same instance
    template <typename Singleton>
    static void verifyInstance() {
        Singleton& instance1 = Singleton::getInstance();
        Singleton& instance2 = Singleton::getInstance();
        assert(&instance1 == &instance2 && "Singleton should return the same instance");
    }

    // Verifies that copy and move constructors are deleted
    template <typename Singleton>
    static void verifyNoCopyOrMove() {
        static_assert(!std::is_copy_constructible<Singleton>::value, "Singleton should not be copy constructible");
        static_assert(!std::is_move_constructible<Singleton>::value, "Singleton should not be move constructible");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_SINGLETON_TESTER_HPP

