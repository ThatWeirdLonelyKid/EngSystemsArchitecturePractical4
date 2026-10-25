#include "farm_management_service/repository/farm_repository.hpp"

#include <algorithm>
#include <cstdio>
#include <ctime>

namespace ozagrimate::farm_management::repository {

namespace {
std::string nowIso8601() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return buf;
}
}  // namespace

FarmRepository::FarmRepository() {
    farms_ = {
        {"farm-001", "Murray Valley Orchards", "VIC", 120, {"almonds"}, "user-101",
         "2026-09-01T08:00:00Z", ""},
        {"farm-002", "Goulburn Grain Co", "NSW", 340, {"wheat", "canola"}, "user-102",
         "2026-09-03T08:00:00Z", ""},
        {"farm-003", "Riverina Rice Farm", "NSW", 210, {"rice"}, "user-103",
         "2026-09-10T08:00:00Z", ""},
    };
}

std::vector<Farm> FarmRepository::findAll() const { return farms_; }

std::optional<Farm> FarmRepository::findById(const std::string& id) const {
    for (const auto& farm : farms_) {
        if (farm.id == id) return farm;
    }
    return std::nullopt;
}

Farm FarmRepository::create(Farm farm) {
    char idBuf[16];
    std::snprintf(idBuf, sizeof(idBuf), "farm-%03d", nextId_++);
    farm.id = idBuf;
    farm.createdAt = nowIso8601();
    farms_.push_back(farm);
    return farm;
}

std::optional<Farm> FarmRepository::update(const Farm& farm) {
    for (auto& existing : farms_) {
        if (existing.id == farm.id) {
            existing = farm;
            existing.updatedAt = nowIso8601();
            return existing;
        }
    }
    return std::nullopt;
}

bool FarmRepository::remove(const std::string& id) {
    auto it = std::remove_if(farms_.begin(), farms_.end(),
                             [&](const Farm& f) { return f.id == id; });
    if (it == farms_.end()) return false;
    farms_.erase(it, farms_.end());
    return true;
}

}  // namespace ozagrimate::farm_management::repository