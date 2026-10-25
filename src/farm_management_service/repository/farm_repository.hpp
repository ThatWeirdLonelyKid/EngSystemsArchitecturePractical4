#pragma once

#include <optional>
#include <string>
#include <vector>

namespace ozagrimate::farm_management::repository {

struct Farm {
    std::string id;
    std::string name;
    std::string state;
    double areaHectares = 0;
    std::vector<std::string> crops;
    std::string ownerId;
    std::string createdAt;
    std::string updatedAt;  // empty until the farm is updated
};

class FarmRepository {
public:
    FarmRepository();

    std::vector<Farm> findAll() const;
    std::optional<Farm> findById(const std::string& id) const;
    Farm create(Farm farm);                       // assigns id + createdAt
    std::optional<Farm> update(const Farm& farm);  // sets updatedAt, empty if id not found
    bool remove(const std::string& id);

private:
    std::vector<Farm> farms_;
    int nextId_ = 4;
};

}  // namespace ozagrimate::farm_management::repository