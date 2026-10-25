#include "farm_management_service/controller/farm_controller.hpp"

#include <httplib.h>
#include <json.hpp>

namespace ozagrimate::farm_management::controller {

using nlohmann::json;
using service::Farm;

namespace {

void sendJson(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(2), "application/json");
}

json toJson(const Farm& farm) {
    json j = {{"id", farm.id},
              {"name", farm.name},
              {"state", farm.state},
              {"areaHectares", farm.areaHectares},
              {"crops", farm.crops},
              {"ownerId", farm.ownerId},
              {"createdAt", farm.createdAt}};
    if (!farm.updatedAt.empty()) j["updatedAt"] = farm.updatedAt;
    return j;
}

bool parseBody(const httplib::Request& req, json& out) {
    out = json::parse(req.body, nullptr, false);  // false = don't throw
    return !out.is_discarded() && out.is_object();
}

// Copies any fields present in the body onto `farm`. Returns an error message
// (empty = fine). Used by POST (all fields) and PUT (only the fields sent).
std::string applyFields(const json& body, Farm& farm) {
    if (body.contains("name")) {
        if (!body["name"].is_string()) return "name must be a string";
        farm.name = body["name"].get<std::string>();
    }
    if (body.contains("state")) {
        if (!body["state"].is_string()) return "state must be a string";
        farm.state = body["state"].get<std::string>();
    }
    if (body.contains("areaHectares")) {
        if (!body["areaHectares"].is_number()) return "areaHectares must be a number";
        farm.areaHectares = body["areaHectares"].get<double>();
    }
    if (body.contains("ownerId")) {
        if (!body["ownerId"].is_string()) return "ownerId must be a string";
        farm.ownerId = body["ownerId"].get<std::string>();
    }
    if (body.contains("crops")) {
        if (!body["crops"].is_array()) return "crops must be an array of strings";
        farm.crops.clear();
        for (const auto& c : body["crops"]) {
            if (!c.is_string()) return "crops must be an array of strings";
            farm.crops.push_back(c.get<std::string>());
        }
    }
    return "";
}

}  // namespace

FarmController::FarmController(service::FarmService& service) : service_(service) {}

void FarmController::registerRoutes(httplib::Server& server) {
    server.Get("/farms", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetAll(req, res);
    });
    server.Post("/farms", [this](const httplib::Request& req, httplib::Response& res) {
        handleCreate(req, res);
    });
    // Regex route: req.matches[1] holds the {id} part
    server.Put(R"(/farms/([^/]+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleUpdate(req, res);
    });
    server.Delete(R"(/farms/([^/]+))", [this](const httplib::Request& req, httplib::Response& res) {
        handleDelete(req, res);
    });
}

// GET /farms -> 200
void FarmController::handleGetAll(const httplib::Request&, httplib::Response& res) const {
    json farms = json::array();
    for (const auto& farm : service_.getAllFarms()) farms.push_back(toJson(farm));
    sendJson(res, 200, {{"farms", farms}, {"total", farms.size()}});
}

// POST /farms -> 201 | 400 bad JSON / invalid fields
void FarmController::handleCreate(const httplib::Request& req, httplib::Response& res) const {
    json body;
    if (!parseBody(req, body)) {
        sendJson(res, 400, {{"error", "Request body must be a valid JSON object"}});
        return;
    }

    Farm farm;
    std::string error = applyFields(body, farm);
    if (error.empty()) error = service_.validate(farm);
    if (!error.empty()) {
        sendJson(res, 400, {{"error", error}});
        return;
    }

    Farm created = service_.createFarm(farm);
    res.set_header("Location", "/farms/" + created.id);
    sendJson(res, 201, toJson(created));
}

// PUT /farms/{id} -> 200 | 400 invalid body | 404 unknown id
void FarmController::handleUpdate(const httplib::Request& req, httplib::Response& res) const {
    const std::string id = req.matches[1];

    auto existing = service_.getFarm(id);
    if (!existing) {
        sendJson(res, 404, {{"error", "Farm not found"}, {"id", id}});
        return;
    }

    json body;
    if (!parseBody(req, body)) {
        sendJson(res, 400, {{"error", "Request body must be a valid JSON object"}});
        return;
    }

    Farm updated = *existing;
    std::string error = applyFields(body, updated);
    if (error.empty()) error = service_.validate(updated);
    if (!error.empty()) {
        sendJson(res, 400, {{"error", error}});
        return;
    }

    auto saved = service_.updateFarm(updated);
    sendJson(res, 200, toJson(*saved));
}

// DELETE /farms/{id} -> 204 | 404 unknown id
void FarmController::handleDelete(const httplib::Request& req, httplib::Response& res) const {
    const std::string id = req.matches[1];

    if (!service_.deleteFarm(id)) {
        sendJson(res, 404, {{"error", "Farm not found"}, {"id", id}});
        return;
    }
    res.status = 204;  // No Content: success, empty body
}

}  // namespace ozagrimate::farm_management::controller