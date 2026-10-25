#include "ai_recommendation_service/rest_controller/recommendation_controller.hpp"

#include <httplib.h>
#include <json.hpp>

namespace ozagrimate::ai_recommendation::rest_controller {

using nlohmann::json;

namespace {
void sendJson(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}
}  

RecommendationController::RecommendationController(
    data_repository::RecommendationRepository& repository)
    : repository_(repository) {}

void RecommendationController::registerRoutes(httplib::Server& server) {
    server.Get("/recommendations",
               [this](const httplib::Request& req, httplib::Response& res) {
                   handleGetRecommendations(req, res);
               });
}

// GET /recommendations?farmId=farm-001
//   200 approved recommendations | 400 missing farmId | 404 unknown farm
void RecommendationController::handleGetRecommendations(const httplib::Request& req,
                                                        httplib::Response& res) const {
    if (!req.has_param("farmId") || req.get_param_value("farmId").empty()) {
        sendJson(res, 400, {{"error", "farmId query parameter is required"}});
        return;
    }

    const std::string farmId = req.get_param_value("farmId");

    if (!repository_.farmExists(farmId)) {
        sendJson(res, 404, {{"error", "Farm not found"}, {"farmId", farmId}});
        return;
    }

    json recommendations = json::array();
    for (const auto& rec : repository_.findApprovedByFarm(farmId)) {
        recommendations.push_back({{"id", rec.id},
                                   {"type", rec.type},
                                   {"summary", rec.summary},
                                   {"explanation", rec.explanation},
                                   {"reviewStatus", rec.reviewStatus},
                                   {"reviewedBy", rec.reviewedBy}});
    }

    sendJson(res, 200, {{"farmId", farmId}, {"recommendations", recommendations}});
}

}