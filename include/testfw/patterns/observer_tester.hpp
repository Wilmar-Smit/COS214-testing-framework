#ifndef TESTFW_PATTERNS_OBSERVER_TESTER_HPP
#define TESTFW_PATTERNS_OBSERVER_TESTER_HPP

#include <cassert>
#include <vector>
#include <string>
#include <iostream>

namespace testfw {
namespace patterns {

// A simple mock observer for testing notification
class MockObserver {
public:
    void notify(const std::string& msg) { notifications.push_back(msg); }
    size_t notificationCount() const { return notifications.size(); }
    void clear() { notifications.clear(); }
    std::vector<std::string> notifications;
};

class ObserverPatternTester {
public:
    template <typename Subject, typename Observer>
    static void verifyRegistration(Subject& subject, Observer* obs) {
        subject.addObserver(obs);
        assert(subject.getObserverCount() > 0 && "Observer should be registered");
    }

    template <typename Subject, typename Observer>
    static void verifyNotification(Subject& subject, Observer* obs, const std::string& msg) {
        obs->clear();
        subject.notifyObservers(msg);
        assert(obs->notificationCount() > 0 && "Observer should receive notification");
    }

    template <typename Subject, typename Observer>
    static void verifyDetachment(Subject& subject, Observer* obs) {
        subject.removeObserver(obs);
        assert(subject.getObserverCount() == 0 && "Observer should be detached");
    }
};

} // namespace patterns
} // namespace testfw

#endif // TESTFW_PATTERNS_OBSERVER_TESTER_HPP
