#ifndef TESTFW_PATTERNS_MEDIATOR_TESTER_HPP
#define TESTFW_PATTERNS_MEDIATOR_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class MediatorPatternTester {
public:
    // Verifies that the mediator coordinates communication between colleagues
    template <typename Mediator, typename Colleague, typename Message, typename Predicate>
    static void verifyCoordination(Mediator& mediator, Colleague& sender, Colleague& receiver, const Message& message, Predicate received) {
        mediator.send(message, &sender, &receiver);
        assert(received() && "Mediator should coordinate communication between colleagues");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_MEDIATOR_TESTER_HPP

