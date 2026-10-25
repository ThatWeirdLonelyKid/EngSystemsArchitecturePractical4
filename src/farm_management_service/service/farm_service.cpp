#include "farm_management_service/service/farm_service.hpp"

namespace ozagrimate::farm_management::service {

FarmService::FarmService(repository::FarmRepository& repository) : repository_(repository) {}

std::vector<Farm> FarmService::getAllFarms() const { return repository_.findAll(); }

std::optional<Farm> FarmService::getFarm(const std::string& id) const {
    return repository_.findById(id);
}

std::string FarmService::validate(const Farm& farm) const {
    if (farm.name.empty()) return "name is required";
    if (farm.state.empty()) return "state is required";
    if (!(farm.areaHectares > 0)) return "areaHectares must be greater than 0";
    return "";
}

Farm FarmService::createFarm(const Farm& farm) { return repository_.create(farm); }

std::optional<Farm> FarmService::updateFarm(const Farm& farm) { return repository_.update(farm); }

bool FarmService::deleteFarm(const std::string& id) { return repository_.remove(id); }

}  // namespace ozagrimate::farm_management::service