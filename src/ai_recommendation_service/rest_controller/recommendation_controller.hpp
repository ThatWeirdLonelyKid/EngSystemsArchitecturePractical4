#pragma once

#include "ai_recommendation_service/data_repository/recommendation_repository.hpp"

namespace httplib {
class Server;
struct Request;
struct Response;
}  

namespace ozagrimate::ai_recommendation::rest_controller {

class RecommendationController {
public:
    explicit RecommendationController(data_repository::RecommendationRepository& repository);

    void registerRoutes(httplib::Server& server);

private:
    void handleGetRecommendations(const httplib::Request& req, httplib::Response& res) const;

    data_repository::RecommendationRepository& repository_;
};

}  