#ifndef TESTFW_PATTERNS_FLYWEIGHT_TESTER_HPP
#define TESTFW_PATTERNS_FLYWEIGHT_TESTER_HPP

#include <cassert>
#include <type_traits>
#include <unordered_set>

namespace testfw {
namespace patterns {

class FlyweightPatternTester {
public:
    // Verifies that the factory returns shared instances for the same key
    template <typename Factory, typename Key, typename Flyweight>
    static void verifySharing(Factory& factory, const Key& key) {
        auto fw1 = factory.getFlyweight(key);
        auto fw2 = factory.getFlyweight(key);
        assert(fw1 == fw2 && "Flyweight factory should return shared instance for the same key");
    }

    // Verifies that the factory creates new instances for different keys
    template <typename Factory, typename Key>
    static void verifyDistinct(Factory& factory, const Key& key1, const Key& key2) {
        auto fw1 = factory.getFlyweight(key1);
        auto fw2 = factory.getFlyweight(key2);
        assert(fw1 != fw2 && "Flyweight factory should return different instances for different keys");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_FLYWEIGHT_TESTER_HPP

