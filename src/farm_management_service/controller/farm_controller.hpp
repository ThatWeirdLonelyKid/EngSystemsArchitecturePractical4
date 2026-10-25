#pragma once

#include "farm_management_service/service/farm_service.hpp"

namespace httplib {
class Server;
struct Request;
struct Response;
}  

namespace ozagrimate::farm_management::controller {

class FarmController {
public:
    explicit FarmController(service::FarmService& service);

    void registerRoutes(httplib::Server& server);

private:
    void handleGetAll(const httplib::Request& req, httplib::Response& res) const;
    void handleCreate(const httplib::Request& req, httplib::Response& res) const;
    void handleUpdate(const httplib::Request& req, httplib::Response& res) const;
    void handleDelete(const httplib::Request& req, httplib::Response& res) const;

    service::FarmService& service_;
};

}  