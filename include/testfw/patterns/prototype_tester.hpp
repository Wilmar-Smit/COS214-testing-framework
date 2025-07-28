#ifndef TESTFW_PATTERNS_PROTOTYPE_TESTER_HPP
#define TESTFW_PATTERNS_PROTOTYPE_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class PrototypePatternTester {
public:
    // Verifies that the clone method returns a new object of the same type
    template <typename Prototype>
    static void verifyClone(const Prototype& prototype) {
        auto clone = prototype.clone();
        assert(clone != nullptr && "Clone should not be null");
        assert(typeid(*clone) == typeid(prototype) && "Clone should be of the same type as prototype");
        assert(clone.get() != &prototype && "Clone should be a different object");
    }

    // Verifies that the clone is a deep copy (user should provide a comparison function)
    template <typename Prototype, typename Compare>
    static void verifyDeepCopy(const Prototype& prototype, Compare compare) {
        auto clone = prototype.clone();
        assert(compare(prototype, *clone) && "Clone should be a deep copy of the prototype");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_PROTOTYPE_TESTER_HPP

