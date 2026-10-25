#include "notification_service/notification_store/notification_repository.hpp"

namespace ozagrimate::notification::notification_store {

NotificationRepository::NotificationRepository() {
    knownUsers_ = {"user-101", "user-102", "user-103"};

    notifications_ = {
        {"notif-700", "user-101", "PRE_WARNING", "Soil moisture trending low in Block A", "PUSH",
         "DELIVERED", "2026-10-25T08:46:00Z"},
        {"notif-701", "user-101", "WARNING", "Soil moisture low in Block A", "PUSH", "DELIVERED",
         "2026-10-25T09:16:00Z"},
        {"notif-702", "user-101", "REMINDER", "Block A still needs irrigation", "SMS", "DELIVERED",
         "2026-10-25T10:16:00Z"},
        {"notif-703", "user-102", "WARNING", "Frost risk tonight for wheat paddocks", "EMAIL",
         "DELIVERED", "2026-10-25T07:00:00Z"},
    };
}

bool NotificationRepository::userExists(const std::string& userId) const {
    return knownUsers_.count(userId) > 0;
}

std::vector<Notification> NotificationRepository::findByUser(const std::string& userId) const {
    std::vector<Notification> result;
    for (const auto& n : notifications_) {
        if (n.userId == userId) result.push_back(n);
    }
    return result;
}

}  // namespace ozagrimate::notification::notification_store