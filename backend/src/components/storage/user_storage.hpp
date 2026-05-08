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
        
        auto it = users_by_id_.find(user.id);
        if (it != users_by_id_.end()) {
            const auto& old_user = it->second;

            if (old_user.email != user.email) {
                email_to_id_.erase(old_user.email);
            }

            if (old_user.name != user.name) {
                username_to_id_.erase(old_user.name);
            }

            if (!old_user.token.empty() && old_user.token != user.token) {
                token_to_id_.erase(old_user.token);
            }
        }

        const std::string& id = user.id;
        
        users_by_id_[id] = user;
        
        username_to_id_[user.name] = id;
        email_to_id_[user.email] = id;
        
        if (!user.token.empty()) {
            token_to_id_[user.token] = id;
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

    std::optional<models::User> FindUserByEmail(const std::string& email) const {
        std::shared_lock lock(mutex_);
        
        auto it_id = email_to_id_.find(email);
        if (it_id == email_to_id_.end()) {
            return std::nullopt;
        }

        auto it_user = users_by_id_.find(it_id->second);
        if (it_user == users_by_id_.end()) {
            return std::nullopt;
        }

        return it_user->second;
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
        
        auto it = users_by_id_.find(id);
        if (it == users_by_id_.end()) { 
            return false; 
        }

        const auto& user = it->second;

        if (!user.token.empty()) {
            token_to_id_.erase(user.token);
        }

        if (!user.name.empty()) {
            username_to_id_.erase(user.name);
        }

        if (!user.email.empty()) {
            email_to_id_.erase(user.email);
        }

        users_by_id_.erase(it);
        
        return true;
    }

private:
    mutable userver::engine::SharedMutex mutex_;
    
    std::unordered_map<std::string, models::User> users_by_id_;
    std::unordered_map<std::string, std::string> username_to_id_;
    std::unordered_map<std::string, std::string> email_to_id_;
    std::unordered_map<std::string, std::string> token_to_id_; 
};

} // namespace agromach::components