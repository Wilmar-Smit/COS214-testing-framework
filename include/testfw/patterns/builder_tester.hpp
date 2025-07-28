#ifndef TESTFW_PATTERNS_BUILDER_TESTER_HPP
#define TESTFW_PATTERNS_BUILDER_TESTER_HPP

#include <cassert>
#include <type_traits>

namespace testfw {
namespace patterns {

class BuilderPatternTester {
public:
    // Verifies that the builder constructs the correct product
    template <typename Builder, typename Director, typename Product, typename... Args>
    static void verifyProduct(Builder& builder, Director& director, const Product& expectedProduct, Args&&... args) {
        director.construct(builder, std::forward<Args>(args)...);
        Product product = builder.getResult();
        assert(product == expectedProduct && "Builder should construct the expected product");
    }

    // Verifies that the builder can reset and build again
    template <typename Builder, typename Director, typename Product, typename... Args>
    static void verifyReset(Builder& builder, Director& director, const Product& expectedProduct, Args&&... args) {
        builder.reset();
        director.construct(builder, std::forward<Args>(args)...);
        Product product = builder.getResult();
        assert(product == expectedProduct && "Builder should construct the expected product after reset");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_BUILDER_TESTER_HPP

