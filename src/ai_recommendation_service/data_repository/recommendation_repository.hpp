#pragma once

#include <set>
#include <string>
#include <vector>

namespace ozagrimate::ai_recommendation::data_repository {

struct Recommendation {
    std::string id;
    std::string farmId;
    std::string type;
    std::string summary;
    std::string explanation;   
    std::string reviewStatus;  
    std::string reviewedBy;
};

class RecommendationRepository {
public:
    RecommendationRepository();

    bool farmExists(const std::string& farmId) const;

    std::vector<Recommendation> findApprovedByFarm(const std::string& farmId) const;

private:
    std::set<std::string> knownFarms_;
    std::vector<Recommendation> recommendations_;
};

}  