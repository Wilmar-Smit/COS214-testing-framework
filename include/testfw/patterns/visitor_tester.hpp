#ifndef TESTFW_PATTERNS_VISITOR_TESTER_HPP
#define TESTFW_PATTERNS_VISITOR_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class VisitorPatternTester {
public:
    // Verifies that the visitor visits all elements and performs the correct operation
    template <typename Element, typename Visitor, typename Predicate>
    static void verifyVisit(Element& element, Visitor& visitor, Predicate wasVisited) {
        element.accept(visitor);
        assert(wasVisited() && "Visitor should visit the element and perform the operation");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_VISITOR_TESTER_HPP

