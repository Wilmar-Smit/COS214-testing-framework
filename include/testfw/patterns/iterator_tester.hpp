#ifndef TESTFW_PATTERNS_ITERATOR_TESTER_HPP
#define TESTFW_PATTERNS_ITERATOR_TESTER_HPP

#include <cassert>
#include <type_traits>
#include <vector>

namespace testfw {
namespace patterns {

class IteratorPatternTester {
public:
    // Verifies that the iterator traverses all elements in the collection
    template <typename Collection, typename Iterator, typename ValueType>
    static void verifyIteration(Collection& collection, const std::vector<ValueType>& expected) {
        std::vector<ValueType> actual;
        for (Iterator it = collection.begin(); it != collection.end(); ++it) {
            actual.push_back(*it);
        }
        assert(actual == expected && "Iterator should traverse all elements in order");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_ITERATOR_TESTER_HPP

