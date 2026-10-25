#pragma once

#include "notification_service/notification_store/notification_repository.hpp"

namespace httplib {
class Server;
struct Request;
struct Response;
}  

namespace ozagrimate::notification::rest_controller {

class NotificationController {
public:
    explicit NotificationController(notification_store::NotificationRepository& repository);

    void registerRoutes(httplib::Server& server);

private:
    void handleGetNotifications(const httplib::Request& req, httplib::Response& res) const;

    notification_store::NotificationRepository& repository_;
};

} 