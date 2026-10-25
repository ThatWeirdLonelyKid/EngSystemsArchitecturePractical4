#include "iot_monitoring_service/sensor_controller/sensor_controller.hpp"

#include <httplib.h>
#include <json.hpp>

namespace ozagrimate::iot_monitoring::sensor_controller {

using nlohmann::json;
using reading_service::AddStatus;
using reading_service::Reading;

namespace {

void sendJson(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}

json toJson(const Reading& r) {
    return {{"id", r.id},
            {"deviceType", r.deviceType},
            {"value", r.value},
            {"unit", r.unit},
            {"recordedAt", r.recordedAt}};
}

bool parseBody(const httplib::Request& req, json& out) {
    out = json::parse(req.body, nullptr, false);
    return !out.is_discarded() && out.is_object();
}

}  // namespace

SensorController::SensorController(reading_service::ReadingService& service)
    : service_(service) {}

void SensorController::registerRoutes(httplib::Server& server) {
    server.Get("/sensors", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetReadings(req, res);
    });
    server.Post("/sensors", [this](const httplib::Request& req, httplib::Response& res) {
        handleAddReading(req, res);
    });
}

// GET /sensors?farmId=farm-001 -> 200 | 400 missing farmId | 404 unknown farm
void SensorController::handleGetReadings(const httplib::Request& req,
                                         httplib::Response& res) const {
    if (!req.has_param("farmId") || req.get_param_value("farmId").empty()) {
        sendJson(res, 400, {{"error", "farmId query parameter is required"}});
        return;
    }
    const std::string farmId = req.get_param_value("farmId");

    if (!service_.farmExists(farmId)) {
        sendJson(res, 404, {{"error", "Farm not found"}, {"farmId", farmId}});
        return;
    }

    json readings = json::array();
    for (const auto& r : service_.getReadings(farmId)) readings.push_back(toJson(r));
    sendJson(res, 200, {{"farmId", farmId}, {"readings", readings}});
}

// POST /sensors -> 201 | 400 bad body / unsupported device | 404 unknown farm
void SensorController::handleAddReading(const httplib::Request& req,
                                        httplib::Response& res) const {
    json body;
    if (!parseBody(req, body)) {
        sendJson(res, 400, {{"error", "Request body must be a valid JSON object"}});
        return;
    }

    // All five fields are required, with the right types.
    const char* stringFields[] = {"farmId", "deviceType", "unit", "recordedAt"};
    for (const char* field : stringFields) {
        if (!body.contains(field) || !body[field].is_string() ||
            body[field].get<std::string>().empty()) {
            sendJson(res, 400, {{"error", std::string(field) + " is required and must be a string"}});
            return;
        }
    }
    if (!body.contains("value") || !body["value"].is_number()) {
        sendJson(res, 400, {{"error", "value is required and must be a number"}});
        return;
    }

    Reading reading;
    reading.farmId = body["farmId"].get<std::string>();
    reading.deviceType = body["deviceType"].get<std::string>();
    reading.value = body["value"].get<double>();
    reading.unit = body["unit"].get<std::string>();
    reading.recordedAt = body["recordedAt"].get<std::string>();

    auto result = service_.addReading(reading);
    switch (result.status) {
        case AddStatus::UnknownFarm:
            sendJson(res, 404, {{"error", "Farm not found"}, {"farmId", reading.farmId}});
            return;
        case AddStatus::UnsupportedDevice:
            sendJson(res, 400, {{"error", "Unsupported deviceType"}, {"deviceType", reading.deviceType}});
            return;
        case AddStatus::Created: {
            json out = toJson(result.reading);
            out["farmId"] = result.reading.farmId;
            out["alertTriggered"] = result.alertTriggered;
            sendJson(res, 201, out);
            return;
        }
    }
}

}  // namespace ozagrimate::iot_monitoring::sensor_controller