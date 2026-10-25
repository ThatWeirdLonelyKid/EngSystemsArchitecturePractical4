#pragma once

#include <string>

namespace ozagrimate::iot_monitoring::threshold_evaluator {

// Decides whether a sensor reading breaches an alert criterion.
class ThresholdEvaluator {
public:
    bool isBreach(const std::string& deviceType, double value) const;
};

}  // namespace ozagrimate::iot_monitoring::threshold_evaluator