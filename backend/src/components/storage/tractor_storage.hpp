#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <unordered_map>
#include <vector>
#include "../../models/tractor.hpp"

namespace agromach::components {

class TractorStorage final : public userver::components::LoggableComponentBase {
public:
    static constexpr std::string_view kName = "tractor-storage";

    TractorStorage(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context)
        : LoggableComponentBase(config, context) {}

    void UpdateTractor(models::Tractor tractor) {
        std::unique_lock lock(mutex_);
        storage_[tractor.id] = std::move(tractor);
    }

    std::vector<models::Tractor> GetAllTractors() const {
        std::shared_lock lock(mutex_);
        std::vector<models::Tractor> result;
        result.reserve(storage_.size());
        for (const auto& [id, tractor] : storage_) {
            result.push_back(tractor);
        }
        return result;
    }

    bool RemoveTractor(const std::string& id) {
        std::unique_lock lock(mutex_);
        return storage_.erase(id) > 0;
    }

private:
    mutable userver::engine::SharedMutex mutex_;
    std::unordered_map<std::string, models::Tractor> storage_;
};

} // namespace agromach::components