#pragma once

#include <set>
#include <string>
#include <vector>

#include "iot_monitoring_service/threshold_evaluator/threshold_evaluator.hpp"

namespace ozagrimate::iot_monitoring::reading_service {

struct Reading {
    std::string id;
    std::string farmId;
    std::string deviceType;
    double value = 0;
    std::string unit;
    std::string recordedAt;
};

enum class AddStatus { Created, UnknownFarm, UnsupportedDevice };

struct AddResult {
    AddStatus status;
    Reading reading;
    bool alertTriggered = false;
};

// Validates and stores readings (in memory) and asks the evaluator about alerts.
class ReadingService {
public:
    explicit ReadingService(threshold_evaluator::ThresholdEvaluator& evaluator);

    bool farmExists(const std::string& farmId) const;
    std::vector<Reading> getReadings(const std::string& farmId) const;
    AddResult addReading(Reading reading);

private:
    threshold_evaluator::ThresholdEvaluator& evaluator_;
    std::set<std::string> knownFarms_;
    std::set<std::string> supportedDevices_;  // 10 device types (ASR2)
    std::vector<Reading> readings_;
    int nextId_ = 502;
};

}  // namespace ozagrimate::iot_monitoring::reading_service