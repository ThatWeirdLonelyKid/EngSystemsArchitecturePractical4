#include "ai_recommendation_service/data_repository/recommendation_repository.hpp"

namespace ozagrimate::ai_recommendation::data_repository {

RecommendationRepository::RecommendationRepository() {
    knownFarms_ = {"farm-001", "farm-002", "farm-003"};

    recommendations_ = {
        {"rec-301", "farm-001", "irrigation",
         "Reduce irrigation by 20% in Block A",
         "Soil moisture is high and rain is forecast within 48 hours.",
         "APPROVED", "agronomist-12"},
        // Not approved yet, so it must NOT be returned to farmers.
        {"rec-302", "farm-001", "fertiliser",
         "Apply nitrogen fertiliser to Block B",
         "Soil nitrogen readings are below the target range.",
         "PENDING_REVIEW", ""},
        {"rec-303", "farm-002", "pest control",
         "Inspect wheat crop for aphids this week",
         "Warm, dry conditions increase aphid activity in your region.",
         "APPROVED", "agronomist-07"},
    };
}

bool RecommendationRepository::farmExists(const std::string& farmId) const {
    return knownFarms_.count(farmId) > 0;
}

std::vector<Recommendation> RecommendationRepository::findApprovedByFarm(
    const std::string& farmId) const {
    std::vector<Recommendation> result;
    for (const auto& rec : recommendations_) {
        if (rec.farmId == farmId && rec.reviewStatus == "APPROVED") {
            result.push_back(rec);
        }
    }
    return result;
}

}