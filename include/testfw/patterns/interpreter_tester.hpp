#ifndef TESTFW_PATTERNS_INTERPRETER_TESTER_HPP
#define TESTFW_PATTERNS_INTERPRETER_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class InterpreterPatternTester {
public:
    // Verifies that the interpreter evaluates expressions correctly
    template <typename Expression, typename Context, typename ResultType>
    static void verifyInterpret(Expression& expr, Context& context, const ResultType& expected) {
        auto result = expr.interpret(context);
        assert(result == expected && "Interpreter should evaluate expression to expected result");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_INTERPRETER_TESTER_HPP

