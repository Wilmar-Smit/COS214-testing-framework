#ifndef TESTFW_PATTERNS_STRATEGY_TESTER_HPP
#define TESTFW_PATTERNS_STRATEGY_TESTER_HPP

#include <memory>
#include <string>
#include <type_traits>
#include <cassert>

namespace testfw {
namespace patterns {

// Example interface for verifying Strategy pattern compliance
class StrategyPatternTester {
public:
    template <typename Context, typename StrategyBase, typename ConcreteStrategy1, typename ConcreteStrategy2>
    static void verify(Context& context,
                       std::unique_ptr<StrategyBase> strat1,
                       std::unique_ptr<StrategyBase> strat2,
                       const std::string& expectedType1,
                       const std::string& expectedType2) {
        // Set first strategy and check type
        context.setPaymentStrategy(std::move(strat1));
        assert(context.getCurrentPaymentType() == expectedType1);
        // Set second strategy and check type
        context.setPaymentStrategy(std::move(strat2));
        assert(context.getCurrentPaymentType() == expectedType2);
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_STRATEGY_TESTER_HPP

