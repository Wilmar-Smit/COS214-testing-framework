#ifndef TESTFW_PATTERNS_COMPOSITE_TESTER_HPP
#define TESTFW_PATTERNS_COMPOSITE_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class CompositePatternTester {
public:
    // Verifies that a composite can add and contain children
    template <typename Composite, typename Component>
    static void verifyAddChild(Composite& composite, Component* child) {
        size_t before = composite.childCount();
        composite.add(child);
        size_t after = composite.childCount();
        assert(after == before + 1 && "Composite should increase child count after add");
    }

    // Verifies that a composite can remove children
    template <typename Composite, typename Component>
    static void verifyRemoveChild(Composite& composite, Component* child) {
        size_t before = composite.childCount();
        composite.remove(child);
        size_t after = composite.childCount();
        assert(after == before - 1 && "Composite should decrease child count after remove");
    }

    // Verifies that operation is propagated to children
    template <typename Composite>
    static void verifyOperationPropagation(Composite& composite) {
        // This assumes Composite and its children have an 'operation' method
        // and that children record invocation (user must provide such a mock)
        composite.operation();
        // User should check their mock for invocation
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_COMPOSITE_TESTER_HPP

