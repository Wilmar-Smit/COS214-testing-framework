# Pattern Verification Helpers

This document explains how to use the pattern-specific test helpers provided by the framework to verify correct implementation of design patterns in academic assignments.

## Table of Contents
- [Strategy Pattern Verification](#strategy-pattern-verification)
- [Observer Pattern Verification](#observer-pattern-verification)
- [Factory Pattern Verification](#factory-pattern-verification)
- [Singleton Pattern Verification](#singleton-pattern-verification)
- [Decorator Pattern Verification](#decorator-pattern-verification)
- [Adapter Pattern Verification](#adapter-pattern-verification)
- [Composite Pattern Verification](#composite-pattern-verification)
- [Template Method Pattern Verification](#template-method-pattern-verification)
- [Command Pattern Verification](#command-pattern-verification)
- [State Pattern Verification](#state-pattern-verification)
- [Bridge Pattern Verification](#bridge-pattern-verification)
- [Proxy Pattern Verification](#proxy-pattern-verification)
- [Builder Pattern Verification](#builder-pattern-verification)
- [Prototype Pattern Verification](#prototype-pattern-verification)
- [Chain of Responsibility Pattern Verification](#chain-of-responsibility-pattern-verification)
- [Mediator Pattern Verification](#mediator-pattern-verification)
- [Memento Pattern Verification](#memento-pattern-verification)
- [Interpreter Pattern Verification](#interpreter-pattern-verification)
- [Iterator Pattern Verification](#iterator-pattern-verification)
- [Visitor Pattern Verification](#visitor-pattern-verification)
- [Flyweight Pattern Verification](#flyweight-pattern-verification)

---

## Strategy Pattern Verification

**Purpose:** Ensure your context class can switch strategies at runtime and that each strategy behaves as expected.

The `StrategyPatternTester` helps you verify:
- Runtime switching of strategies
- Correct interface implementation
- Polymorphic behavior

### Example Usage
```cpp
#include <testfw/patterns/strategy_tester.hpp>
#include "../src/PaymentStrategy.hpp"

TEST_CASE("Strategy Pattern - Runtime Switching") {
    PaymentProcessor processor;
    auto strat1 = std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "12/25");
    auto strat2 = std::make_unique<PayPalPayment>("user@example.com");
    testfw::patterns::StrategyPatternTester::verify(
        processor,
        std::move(strat1),
        std::move(strat2),
        "Credit Card",
        "PayPal"
    );
}
```

---

## Observer Pattern Verification

**Purpose:** Ensure your subject class can register, notify, and detach observers correctly.

The `ObserverPatternTester` and `MockObserver` help you verify:
- Observer registration
- Notification mechanism
- Observer detachment

### Example Usage
```cpp
#include <testfw/patterns/observer_tester.hpp>
#include "../src/WeatherStation.hpp"

TEST_CASE("Observer Pattern - Registration and Notification") {
    WeatherStation station;
    testfw::patterns::MockObserver obs;
    testfw::patterns::ObserverPatternTester::verifyRegistration(station, &obs);
    testfw::patterns::ObserverPatternTester::verifyNotification(station, &obs, "update");
    testfw::patterns::ObserverPatternTester::verifyDetachment(station, &obs);
}
```

---

## Factory Pattern Verification

**Purpose:** Ensure your factory class creates objects of the correct type and supports multiple product types.

The `FactoryPatternTester` helps you verify:
- Correct product creation
- Support for multiple product types

### Example Usage
```cpp
#include <testfw/patterns/factory_tester.hpp>
#include "../src/ShapeFactory.hpp"

TEST_CASE("Factory Pattern - Single Product Creation") {
    ShapeFactory factory;
    testfw::patterns::FactoryPatternTester::verifyCreation<ShapeFactory, Shape>(
        factory, typeid(Circle).name(), "circle", 5.0
    );
}

TEST_CASE("Factory Pattern - Multiple Product Creation") {
    ShapeFactory factory;
    std::vector<std::string> expectedTypes = {typeid(Circle).name(), typeid(Rectangle).name()};
    std::vector<std::tuple<std::string, double>> argSets = {
        std::make_tuple("circle", 5.0),
        std::make_tuple("rectangle", 3.0)
    };
    testfw::patterns::FactoryPatternTester::verifyMultipleCreation<ShapeFactory, Shape>(
        factory, expectedTypes, argSets
    );
}
```

---

## Singleton Pattern Verification

**Purpose:** Ensure your singleton class enforces the single instance property and disables copy/move operations.

The `SingletonPatternTester` helps you verify:
- Only one instance is ever created
- Copy and move constructors are deleted

### Example Usage
```cpp
#include <testfw/patterns/singleton_tester.hpp>
#include "../src/Logger.hpp"

TEST_CASE("Singleton Pattern - Single Instance") {
    testfw::patterns::SingletonPatternTester::verifyInstance<Logger>();
}

TEST_CASE("Singleton Pattern - No Copy or Move") {
    testfw::patterns::SingletonPatternTester::verifyNoCopyOrMove<Logger>();
}
```

---

## Decorator Pattern Verification

**Purpose:** Ensure your decorator class wraps a component and can extend its behavior.

The `DecoratorPatternTester` helps you verify:
- Correct wrapping of the component
- Extension of behavior via the decorator

### Example Usage
```cpp
#include <testfw/patterns/decorator_tester.hpp>
#include "../src/NotifierDecorator.hpp"

TEST_CASE("Decorator Pattern - Decoration and Extension") {
    ConcreteComponent component;
    NotifierDecorator decorator(&component);
    testfw::patterns::DecoratorPatternTester::verifyDecoration(decorator, &component);
    // To verify extended behavior, use a mock or check side effects
    testfw::patterns::DecoratorPatternTester::verifyExtendedBehavior(decorator);
}
```

---

## Adapter Pattern Verification

**Purpose:** Ensure your adapter class adapts an adaptee to the target interface.

The `AdapterPatternTester` helps you verify:
- Adapter implements the target interface
- Adapter delegates calls to the adaptee

### Example Usage
```cpp
#include <testfw/patterns/adapter_tester.hpp>
#include "../src/LegacyAdapter.hpp"

TEST_CASE("Adapter Pattern - Interface Compliance") {
    LegacyAdaptee adaptee;
    LegacyAdapter adapter(&adaptee);
    testfw::patterns::AdapterPatternTester::verify<LegacyAdapter, TargetInterface>(adapter, adaptee);
}
```

---

## Composite Pattern Verification

**Purpose:** Ensure your composite class can add, remove, and manage child components, and that operations are correctly propagated to children.

The `CompositePatternTester` helps you verify:
- Adding and removing child components
- Correct child count management
- Propagation of operations to children

### Example Usage
```cpp
#include <testfw/patterns/composite_tester.hpp>
#include "../src/CompositeGraphic.hpp"

TEST_CASE("Composite Pattern - Add and Remove Child") {
    CompositeGraphic composite;
    auto* child = new GraphicLeaf();
    testfw::patterns::CompositePatternTester::verifyAddChild(composite, child);
    testfw::patterns::CompositePatternTester::verifyRemoveChild(composite, child);
    delete child;
}

TEST_CASE("Composite Pattern - Operation Propagation") {
    CompositeGraphic composite;
    // Add mock children that record operation calls
    // ...
    testfw::patterns::CompositePatternTester::verifyOperationPropagation(composite);
    // Check that all children received the operation
}
```

---

## Template Method Pattern Verification

**Purpose:** Ensure your template method implementation calls steps in the correct order.

The `TemplateMethodPatternTester` helps you verify:
- Correct sequence of method calls in the template method

### Example Usage
```cpp
#include <testfw/patterns/template_method_tester.hpp>
#include "../src/ConcreteTemplate.hpp"

TEST_CASE("Template Method Pattern - Execution Order") {
    ConcreteTemplate obj;
    std::vector<std::string> expectedOrder = {"step1", "step2", "step3"};
    testfw::patterns::TemplateMethodPatternTester::verifyExecutionOrder(obj, expectedOrder);
}
```

---

## Command Pattern Verification

**Purpose:** Ensure your command objects execute actions and can be undone, affecting the receiver as expected.

The `CommandPatternTester` helps you verify:
- Command execution changes receiver state
- Undo restores previous state

### Example Usage
```cpp
#include <testfw/patterns/command_tester.hpp>
#include "../src/LightCommand.hpp"

TEST_CASE("Command Pattern - Execute and Undo") {
    Light light;
    TurnOnCommand cmd(&light);
    testfw::patterns::CommandPatternTester::verifyExecution(cmd, light, true); // true = on
    testfw::patterns::CommandPatternTester::verifyUndo(cmd, light, false); // false = off
}
```

---

## State Pattern Verification

**Purpose:** Ensure your context transitions between states correctly in response to actions.

The `StatePatternTester` helps you verify:
- State transitions after actions
- Correct state after a sequence of actions

### Example Usage
```cpp
#include <testfw/patterns/state_tester.hpp>
#include "../src/Context.hpp"

TEST_CASE("State Pattern - Single Transition") {
    Context ctx;
    testfw::patterns::StatePatternTester::verifyTransition(ctx, "Locked", &Context::lock);
}

TEST_CASE("State Pattern - Multiple Transitions") {
    Context ctx;
    std::vector<std::string> expectedStates = {"Locked", "Unlocked", "Locked"};
    std::vector<std::function<void(Context&)>> actions = {
        [](Context& c){ c.lock(); },
        [](Context& c){ c.unlock(); },
        [](Context& c){ c.lock(); }
    };
    testfw::patterns::StatePatternTester::verifyTransitions(ctx, expectedStates, actions);
}
```

---

## Bridge Pattern Verification

**Purpose:** Ensure your abstraction delegates to the implementor and allows runtime switching.

The `BridgePatternTester` helps you verify:
- Delegation from abstraction to implementor
- Runtime implementor switching

### Example Usage
```cpp
#include <testfw/patterns/bridge_tester.hpp>
#include "../src/RemoteControl.hpp"

TEST_CASE("Bridge Pattern - Delegation") {
    TV tv;
    RemoteControl remote(&tv);
    testfw::patterns::BridgePatternTester::verifyDelegation(remote, &tv);
}

TEST_CASE("Bridge Pattern - Runtime Switch") {
    TV tv;
    Radio radio;
    RemoteControl remote(&tv);
    testfw::patterns::BridgePatternTester::verifyRuntimeSwitch(remote, &radio);
}
```

---

## Proxy Pattern Verification

**Purpose:** Ensure your proxy delegates to the real subject and can control access.

The `ProxyPatternTester` helps you verify:
- Delegation to the real subject
- Access control logic (requires a mock or side effect check)

### Example Usage
```cpp
#include <testfw/patterns/proxy_tester.hpp>
#include "../src/InternetProxy.hpp"

TEST_CASE("Proxy Pattern - Delegation") {
    RealInternet real;
    InternetProxy proxy(&real);
    testfw::patterns::ProxyPatternTester::verifyDelegation(proxy, real);
}

TEST_CASE("Proxy Pattern - Access Control") {
    InternetProxy proxy(nullptr);
    // Use a mock or check side effects for access control
    testfw::patterns::ProxyPatternTester::verifyAccessControl(proxy);
}
```

---

## Builder Pattern Verification

**Purpose:** Ensure your builder constructs the correct product and can reset for reuse.

The `BuilderPatternTester` helps you verify:
- Correct product construction
- Builder reset and reuse

### Example Usage
```cpp
#include <testfw/patterns/builder_tester.hpp>
#include "../src/HouseBuilder.hpp"

TEST_CASE("Builder Pattern - Product Construction") {
    HouseBuilder builder;
    Director director;
    House expected = {/*...*/};
    testfw::patterns::BuilderPatternTester::verifyProduct(builder, director, expected);
}

TEST_CASE("Builder Pattern - Reset and Reuse") {
    HouseBuilder builder;
    Director director;
    House expected = {/*...*/};
    testfw::patterns::BuilderPatternTester::verifyReset(builder, director, expected);
}
```

---

## Prototype Pattern Verification

**Purpose:** Ensure your prototype class supports correct cloning and deep copy semantics.

The `PrototypePatternTester` helps you verify:
- Cloning returns a new object of the same type
- Deep copy correctness (with a user-provided comparison)

### Example Usage
```cpp
#include <testfw/patterns/prototype_tester.hpp>
#include "../src/ShapePrototype.hpp"

TEST_CASE("Prototype Pattern - Clone") {
    CirclePrototype circle(5.0);
    testfw::patterns::PrototypePatternTester::verifyClone(circle);
}

TEST_CASE("Prototype Pattern - Deep Copy") {
    CirclePrototype circle(5.0);
    testfw::patterns::PrototypePatternTester::verifyDeepCopy(circle, [](const auto& a, const auto& b) {
        return a.getRadius() == b.getRadius();
    });
}
```

---

## Chain of Responsibility Pattern Verification

**Purpose:** Ensure your chain passes requests and handles them at the correct handler.

The `ChainOfResponsibilityPatternTester` helps you verify:
- Correct handler processes the request
- Requests are passed along the chain if not handled

### Example Usage
```cpp
#include <testfw/patterns/chain_of_responsibility_tester.hpp>
#include "../src/SupportChain.hpp"

TEST_CASE("Chain of Responsibility - Handling") {
    SupportChain chain;
    testfw::patterns::ChainOfResponsibilityPatternTester::verifyHandling(chain, "password reset", "Handled by Level 1");
}

TEST_CASE("Chain of Responsibility - Passing") {
    SupportChain chain;
    bool wasPassed = false;
    testfw::patterns::ChainOfResponsibilityPatternTester::verifyChainPassing(chain, "unknown request", [&]() { return wasPassed; });
}
```

---

## Mediator Pattern Verification

**Purpose:** Ensure your mediator coordinates communication between colleagues.

The `MediatorPatternTester` helps you verify:
- Mediator coordinates message passing

### Example Usage
```cpp
#include <testfw/patterns/mediator_tester.hpp>
#include "../src/ChatMediator.hpp"

TEST_CASE("Mediator Pattern - Coordination") {
    ChatMediator mediator;
    User alice("Alice");
    User bob("Bob");
    bool received = false;
    testfw::patterns::MediatorPatternTester::verifyCoordination(mediator, alice, bob, "Hello", [&]() { return received; });
}
```

---

## Memento Pattern Verification

**Purpose:** Ensure your originator can save and restore its state using mementos.

The `MementoPatternTester` helps you verify:
- Saving and restoring state

### Example Usage
```cpp
#include <testfw/patterns/memento_tester.hpp>
#include "../src/TextEditor.hpp"

TEST_CASE("Memento Pattern - Save and Restore") {
    TextEditor editor;
    testfw::patterns::MementoPatternTester::verifySaveRestore(editor, "Hello", "World");
}
```

---

## Interpreter Pattern Verification

**Purpose:** Ensure your interpreter evaluates expressions correctly.

The `InterpreterPatternTester` helps you verify:
- Expression evaluation

### Example Usage
```cpp
#include <testfw/patterns/interpreter_tester.hpp>
#include "../src/Expression.hpp"

TEST_CASE("Interpreter Pattern - Evaluation") {
    Context ctx;
    AddExpression expr(1, 2);
    testfw::patterns::InterpreterPatternTester::verifyInterpret(expr, ctx, 3);
}
```

---

## Iterator Pattern Verification

**Purpose:** Ensure your iterator traverses all elements in the collection in order.

The `IteratorPatternTester` helps you verify:
- Complete and ordered traversal

### Example Usage
```cpp
#include <testfw/patterns/iterator_tester.hpp>
#include "../src/NumberCollection.hpp"

TEST_CASE("Iterator Pattern - Traversal") {
    NumberCollection collection = {1, 2, 3};
    std::vector<int> expected = {1, 2, 3};
    testfw::patterns::IteratorPatternTester::verifyIteration<NumberCollection, NumberCollection::iterator, int>(collection, expected);
}
```

---

## Visitor Pattern Verification

**Purpose:** Ensure your visitor visits all elements and performs the correct operation.

The `VisitorPatternTester` helps you verify:
- Visitor visits elements and performs operations

### Example Usage
```cpp
#include <testfw/patterns/visitor_tester.hpp>
#include "../src/Element.hpp"

TEST_CASE("Visitor Pattern - Visit") {
    Element element;
    ConcreteVisitor visitor;
    bool wasVisited = false;
    testfw::patterns::VisitorPatternTester::verifyVisit(element, visitor, [&]() { return wasVisited; });
}
```

---

## Flyweight Pattern Verification

**Purpose:** Ensure your flyweight factory shares instances for the same key and creates new ones for different keys.

The `FlyweightPatternTester` helps you verify:
- Shared instance for same key
- Distinct instances for different keys

### Example Usage
```cpp
#include <testfw/patterns/flyweight_tester.hpp>
#include "../src/FlyweightFactory.hpp"

TEST_CASE("Flyweight Pattern - Sharing") {
    FlyweightFactory factory;
    testfw::patterns::FlyweightPatternTester::verifySharing(factory, "A");
}

TEST_CASE("Flyweight Pattern - Distinct Instances") {
    FlyweightFactory factory;
    testfw::patterns::FlyweightPatternTester::verifyDistinct(factory, "A", "B");
}
```

---

For more examples, see the academic usage section in the main documentation.
