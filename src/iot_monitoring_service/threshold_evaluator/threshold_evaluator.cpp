#include "iot_monitoring_service/threshold_evaluator/threshold_evaluator.hpp"

namespace ozagrimate::iot_monitoring::threshold_evaluator {

bool ThresholdEvaluator::isBreach(const std::string& deviceType, double value) const {
    if (deviceType == "soil-moisture") return value < 25.0;           // % - too dry
    if (deviceType == "air-temperature") return value > 40.0;         // degrees C - heat stress
    if (deviceType == "soil-ph") return value < 5.5 || value > 8.0;   // outside healthy range
    return false;  // other device types have no rule yet
}

}  // namespace ozagrimate::iot_monitoring::threshold_evaluator