#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/engine/shared_mutex.hpp>
#include <unordered_map>
#include <optional>
#include <vector>

#include "../../models/user.hpp"

namespace agromach::components {

class UserStorage final : public userver::components::LoggableComponentBase {
public:
    static constexpr std::string_view kName = "user-storage";

    UserStorage(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context)
        : LoggableComponentBase(config, context) {}

    void UpsertUser(models::User user) {
        std::unique_lock lock(mutex_);
        
        const std::string id = user.id;
        const std::string username = user.name;
        const std::string token = user.token;
        
        users_by_id_[id] = user;
        username_to_id_[username] = id;
        
        if (!token.empty()) {
            token_to_id_[token] = id;
        }
    }

    std::optional<models::User> GetUserByToken(const std::string& token) const {
        std::shared_lock lock(mutex_);
        
        auto it_id = token_to_id_.find(token);
        if (it_id == token_to_id_.end()) {
            return std::nullopt;
        }

        auto it_user = users_by_id_.find(it_id->second);
        if (it_user == users_by_id_.end()) {
            return std::nullopt;
        }

        return it_user->second;
    }

    std::optional<models::User> GetUserById(const std::string& id) const {
        std::shared_lock lock(mutex_);
        if (auto it = users_by_id_.find(id); it != users_by_id_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::vector<models::User> GetAllUsers() const {
        std::shared_lock lock(mutex_);
        std::vector<models::User> all_users;
        all_users.reserve(users_by_id_.size());
        
        for (const auto& [id, user] : users_by_id_) {
            all_users.push_back(user);
        }
        return all_users;
    }

    std::optional<models::User> GetUserByUsername(const std::string& username) const {
        std::shared_lock lock(mutex_);
        
        auto it_id = username_to_id_.find(username);
        if (it_id == username_to_id_.end()) {
            return std::nullopt;
        }

        auto it_user = users_by_id_.find(it_id->second);
        if (it_user == users_by_id_.end()) {
            return std::nullopt;
        }

        return it_user->second;
    }

    bool RemoveUser(const std::string& id) {
        std::unique_lock lock(mutex_);
        
        if (auto it = users_by_id_.find(id); it != users_by_id_.end()) { 
            token_to_id_.erase(it->second.token);
            username_to_id_.erase(it->second.name);
            users_by_id_.erase(it);
            return true;
        }
        return false;
    }

private:
    mutable std::shared_mutex mutex_;
    
    std::unordered_map<std::string, models::User> users_by_id_;
    
    std::unordered_map<std::string, std::string> username_to_id_;
    std::unordered_map<std::string, std::string> token_to_id_; 
};

} // namespace agromach::components