#ifndef TESTFW_PATTERNS_CHAIN_OF_RESPONSIBILITY_TESTER_HPP
#define TESTFW_PATTERNS_CHAIN_OF_RESPONSIBILITY_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class ChainOfResponsibilityPatternTester {
public:
    // Verifies that a request is handled by the correct handler in the chain
    template <typename Handler, typename Request, typename ResultType>
    static void verifyHandling(Handler& chain, const Request& request, const ResultType& expectedResult) {
        auto result = chain.handle(request);
        assert(result == expectedResult && "Chain should handle request and return expected result");
    }

    // Verifies that the chain passes the request along if not handled
    template <typename Handler, typename Request, typename Predicate>
    static void verifyChainPassing(Handler& chain, const Request& request, Predicate wasPassed) {
        chain.handle(request);
        assert(wasPassed() && "Request should be passed along the chain if not handled");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_CHAIN_OF_RESPONSIBILITY_TESTER_HPP

