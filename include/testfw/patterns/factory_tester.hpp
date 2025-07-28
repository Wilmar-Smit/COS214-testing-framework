#ifndef TESTFW_PATTERNS_FACTORY_TESTER_HPP
#define TESTFW_PATTERNS_FACTORY_TESTER_HPP

#include <cassert>
#include <type_traits>
#include <memory>

namespace testfw {
namespace patterns {

class FactoryPatternTester {
public:
    // Verifies that the factory creates an object of the expected type
    template <typename Factory, typename Product, typename... Args>
    static void verifyCreation(Factory& factory, const std::string& expectedType, Args&&... args) {
        std::unique_ptr<Product> product = factory.create(std::forward<Args>(args)...);
        assert(product && "Factory should create a non-null product");
        assert(typeid(*product).name() == expectedType && "Factory should create the correct product type");
    }

    // Verifies that the factory supports multiple product types
    template <typename Factory, typename Product, typename ArgSetContainer>
    static void verifyMultipleCreation(Factory& factory, const std::vector<std::string>& expectedTypes, const ArgSetContainer& argSets) {
        size_t i = 0;
        for (const auto& args : argSets) {
            std::unique_ptr<Product> product = std::apply([&](auto&&... unpackedArgs) {
                return factory.create(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
            }, args);
            assert(product && "Factory should create a non-null product");
            assert(typeid(*product).name() == expectedTypes[i] && "Factory should create the correct product type");
            ++i;
        }
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_FACTORY_TESTER_HPP

