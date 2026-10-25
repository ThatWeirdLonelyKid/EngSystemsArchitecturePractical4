#include <httplib.h>

#include <iostream>

#include "ai_recommendation_service/data_repository/recommendation_repository.hpp"
#include "ai_recommendation_service/rest_controller/recommendation_controller.hpp"
#include "farm_management_service/controller/farm_controller.hpp"
#include "farm_management_service/repository/farm_repository.hpp"
#include "farm_management_service/service/farm_service.hpp"
#include "iot_monitoring_service/reading_service/reading_service.hpp"
#include "iot_monitoring_service/sensor_controller/sensor_controller.hpp"
#include "iot_monitoring_service/threshold_evaluator/threshold_evaluator.hpp"
#include "notification_service/notification_store/notification_repository.hpp"
#include "notification_service/rest_controller/notification_controller.hpp"

int main() {
    using namespace ozagrimate;

    httplib::Server server;

    farm_management::repository::FarmRepository farmRepo;
    farm_management::service::FarmService farmService(farmRepo);
    farm_management::controller::FarmController farmController(farmService);
    farmController.registerRoutes(server);

    iot_monitoring::threshold_evaluator::ThresholdEvaluator evaluator;
    iot_monitoring::reading_service::ReadingService readingService(evaluator);
    iot_monitoring::sensor_controller::SensorController sensorController(readingService);
    sensorController.registerRoutes(server);

    ai_recommendation::data_repository::RecommendationRepository recommendationRepo;
    ai_recommendation::rest_controller::RecommendationController recommendationController(
        recommendationRepo);
    recommendationController.registerRoutes(server);

    notification::notification_store::NotificationRepository notificationRepo;
    notification::rest_controller::NotificationController notificationController(
        notificationRepo);
    notificationController.registerRoutes(server);

    const int port = 8080;
    std::cout << "OzAgriMate running on http://localhost:" << port << std::endl;
    server.listen("0.0.0.0", port);
    return 0;
}