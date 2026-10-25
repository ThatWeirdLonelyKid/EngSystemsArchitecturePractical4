#pragma once

#include <optional>
#include <string>
#include <vector>

#include "farm_management_service/repository/farm_repository.hpp"

namespace ozagrimate::farm_management::service {

using repository::Farm;

class FarmService {
public:
    explicit FarmService(repository::FarmRepository& repository);

    std::vector<Farm> getAllFarms() const;
    std::optional<Farm> getFarm(const std::string& id) const;

    // Returns an error message, or an empty string if the farm is valid.
    std::string validate(const Farm& farm) const;

    Farm createFarm(const Farm& farm);
    std::optional<Farm> updateFarm(const Farm& farm);
    bool deleteFarm(const std::string& id);

private:
    repository::FarmRepository& repository_;
};

}  // namespace ozagrimate::farm_management::service