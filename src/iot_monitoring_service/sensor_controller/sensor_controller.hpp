#pragma once

#include "iot_monitoring_service/reading_service/reading_service.hpp"

namespace httplib {
class Server;
struct Request;
struct Response;
}  

namespace ozagrimate::iot_monitoring::sensor_controller {

class SensorController {
public:
    explicit SensorController(reading_service::ReadingService& service);

    void registerRoutes(httplib::Server& server);

private:
    void handleGetReadings(const httplib::Request& req, httplib::Response& res) const;
    void handleAddReading(const httplib::Request& req, httplib::Response& res) const;

    reading_service::ReadingService& service_;
};

} 