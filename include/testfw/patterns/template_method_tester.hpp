#ifndef TESTFW_PATTERNS_TEMPLATE_METHOD_TESTER_HPP
#define TESTFW_PATTERNS_TEMPLATE_METHOD_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class TemplateMethodPatternTester {
public:
    // Verifies that the template method calls the steps in the correct order
    template <typename TemplateClass, typename... Args>
    static void verifyExecutionOrder(TemplateClass& obj, const std::vector<std::string>& expectedOrder, Args&&... args) {
        // Assumes TemplateClass has a method to get the execution log
        obj.execute(std::forward<Args>(args)...);
        const auto& log = obj.getExecutionLog();
        assert(log == expectedOrder && "Template method should call steps in correct order");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_TEMPLATE_METHOD_TESTER_HPP

