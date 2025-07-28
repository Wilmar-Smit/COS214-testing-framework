#ifndef TESTFW_PATTERNS_MEMENTO_TESTER_HPP
#define TESTFW_PATTERNS_MEMENTO_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class MementoPatternTester {
public:
    // Verifies that the originator can save and restore its state
    template <typename Originator, typename Memento, typename StateType>
    static void verifySaveRestore(Originator& originator, const StateType& state1, const StateType& state2) {
        originator.setState(state1);
        auto memento = originator.save();
        originator.setState(state2);
        originator.restore(memento);
        assert(originator.getState() == state1 && "Originator should restore to saved state");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_MEMENTO_TESTER_HPP

