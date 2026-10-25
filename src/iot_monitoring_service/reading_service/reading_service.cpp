#include "iot_monitoring_service/reading_service/reading_service.hpp"

namespace ozagrimate::iot_monitoring::reading_service {

ReadingService::ReadingService(threshold_evaluator::ThresholdEvaluator& evaluator)
    : evaluator_(evaluator) {
    knownFarms_ = {"farm-001", "farm-002", "farm-003"};
    supportedDevices_ = {"soil-moisture", "soil-temperature", "air-temperature", "humidity",
                         "rainfall",      "wind-speed",       "soil-ph",         "water-flow",
                         "leaf-wetness",  "solar-radiation"};
    readings_ = {
        {"read-501", "farm-001", "soil-moisture", 23.4, "%", "2026-10-25T09:15:00Z"},
        {"read-500", "farm-002", "air-temperature", 28.1, "C", "2026-10-25T09:10:00Z"},
    };
}

bool ReadingService::farmExists(const std::string& farmId) const {
    return knownFarms_.count(farmId) > 0;
}

std::vector<Reading> ReadingService::getReadings(const std::string& farmId) const {
    std::vector<Reading> result;
    for (const auto& r : readings_) {
        if (r.farmId == farmId) result.push_back(r);
    }
    return result;
}

AddResult ReadingService::addReading(Reading reading) {
    if (!farmExists(reading.farmId)) return {AddStatus::UnknownFarm, reading, false};
    if (supportedDevices_.count(reading.deviceType) == 0) {
        return {AddStatus::UnsupportedDevice, reading, false};
    }

    reading.id = "read-" + std::to_string(nextId_++);
    readings_.push_back(reading);

    const bool alert = evaluator_.isBreach(reading.deviceType, reading.value);
    // In the full design this is where the alert event would be published to the queue.
    return {AddStatus::Created, reading, alert};
}

}  // namespace ozagrimate::iot_monitoring::reading_service