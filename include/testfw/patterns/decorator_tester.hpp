#ifndef TESTFW_PATTERNS_DECORATOR_TESTER_HPP
#define TESTFW_PATTERNS_DECORATOR_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class DecoratorPatternTester {
public:
    // Verifies that the decorator wraps the component and delegates operation
    template <typename Decorator, typename Component>
    static void verifyDecoration(Decorator& decorator, Component* component) {
        // Assumes Decorator has a getComponent() method
        assert(decorator.getComponent() == component && "Decorator should wrap the correct component");
    }

    // Verifies that the decorator can extend behavior
    template <typename Decorator, typename... Args>
    static void verifyExtendedBehavior(Decorator& decorator, Args&&... args) {
        // User should provide a mock or check for extended behavior
        decorator.operation(std::forward<Args>(args)...);
        // User should check their mock for extended behavior
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_DECORATOR_TESTER_HPP

