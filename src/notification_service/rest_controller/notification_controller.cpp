#include "notification_service/rest_controller/notification_controller.hpp"

#include <httplib.h>
#include <json.hpp>

namespace ozagrimate::notification::rest_controller {

using nlohmann::json;

namespace {
void sendJson(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}
}  // namespace

NotificationController::NotificationController(
    notification_store::NotificationRepository& repository)
    : repository_(repository) {}

void NotificationController::registerRoutes(httplib::Server& server) {
    server.Get("/notifications", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetNotifications(req, res);
    });
}

// GET /notifications?userId=user-101 -> 200 | 400 missing userId | 404 unknown user
void NotificationController::handleGetNotifications(const httplib::Request& req,
                                                    httplib::Response& res) const {
    if (!req.has_param("userId") || req.get_param_value("userId").empty()) {
        sendJson(res, 400, {{"error", "userId query parameter is required"}});
        return;
    }
    const std::string userId = req.get_param_value("userId");

    if (!repository_.userExists(userId)) {
        sendJson(res, 404, {{"error", "User not found"}, {"userId", userId}});
        return;
    }

    json list = json::array();
    for (const auto& n : repository_.findByUser(userId)) {
        list.push_back({{"id", n.id},
                        {"stage", n.stage},
                        {"message", n.message},
                        {"channel", n.channel},
                        {"status", n.status},
                        {"sentAt", n.sentAt}});
    }
    sendJson(res, 200, {{"userId", userId}, {"notifications", list}});
}

}  // namespace ozagrimate::notification::rest_controller