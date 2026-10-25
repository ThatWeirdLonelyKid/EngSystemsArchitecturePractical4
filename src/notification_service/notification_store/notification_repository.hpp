#pragma once

#include <set>
#include <string>
#include <vector>

namespace ozagrimate::notification::notification_store {

struct Notification {
    std::string id;
    std::string userId;
    std::string stage;    // PRE_WARNING, WARNING or REMINDER (ASR4)
    std::string message;
    std::string channel;  // PUSH, SMS or EMAIL
    std::string status;   // DELIVERED, FAILED or PENDING
    std::string sentAt;
};

// In-memory dummy data only.
class NotificationRepository {
public:
    NotificationRepository();

    bool userExists(const std::string& userId) const;
    std::vector<Notification> findByUser(const std::string& userId) const;

private:
    std::set<std::string> knownUsers_;
    std::vector<Notification> notifications_;
};

}  // namespace ozagrimate::notification::notification_store