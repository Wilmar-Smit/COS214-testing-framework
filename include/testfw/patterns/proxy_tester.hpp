#ifndef TESTFW_PATTERNS_PROXY_TESTER_HPP
#define TESTFW_PATTERNS_PROXY_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class ProxyPatternTester {
public:
    // Verifies that the proxy delegates to the real subject
    template <typename Proxy, typename Subject, typename... Args>
    static void verifyDelegation(Proxy& proxy, Subject& realSubject, Args&&... args) {
        // Assumes Proxy has a getRealSubject() method
        assert(proxy.getRealSubject() == &realSubject && "Proxy should delegate to the correct real subject");
    }

    // Verifies that the proxy can control access (user should provide a mock or check side effects)
    template <typename Proxy, typename... Args>
    static void verifyAccessControl(Proxy& proxy, Args&&... args) {
        // User should provide a mock or check for access control logic
        proxy.request(std::forward<Args>(args)...);
        // Check access control via mock or side effect
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_PROXY_TESTER_HPP

