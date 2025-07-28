#ifndef TESTFW_PATTERNS_BRIDGE_TESTER_HPP
#define TESTFW_PATTERNS_BRIDGE_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class BridgePatternTester {
public:
    // Verifies that the abstraction delegates to the implementor
    template <typename Abstraction, typename Implementor>
    static void verifyDelegation(Abstraction& abstraction, Implementor* implementor) {
        // Assumes Abstraction has a getImplementor() method
        assert(abstraction.getImplementor() == implementor && "Abstraction should delegate to the correct implementor");
    }

    // Verifies that changing the implementor at runtime is possible
    template <typename Abstraction, typename Implementor>
    static void verifyRuntimeSwitch(Abstraction& abstraction, Implementor* newImplementor) {
        abstraction.setImplementor(newImplementor);
        assert(abstraction.getImplementor() == newImplementor && "Abstraction should allow runtime implementor switching");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_BRIDGE_TESTER_HPP

