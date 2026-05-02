#pragma once
#include <string>
#include <optional>
#include <userver/formats/json/value_builder.hpp>

namespace agromach::models {

enum class UserRole { kAdmin, kOperator, kViewer };

struct User {
    std::string id;
    std::string name;
    std::string email;
    std::optional<int> age;
    std::string password_hash;
    std::string token;
    UserRole role;
};

struct UserResponse {
    std::string id;
    std::string username;
    std::string role;
    std::string token;
};

inline std::string_view ToString(UserRole role) {
    switch (role) {
        case UserRole::kAdmin: return "admin";
        case UserRole::kOperator: return "operator";
        case UserRole::kViewer: return "viewer";
    }
    return "unknown";
}

inline UserRole ParseRole(std::string_view role) {
    if (role == "admin") return UserRole::kAdmin;
    if (role == "operator") return UserRole::kOperator;
    return UserRole::kViewer;
}

inline userver::formats::json::Value Serialize(
    const User& user, userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = user.id;
    builder["username"] = user.name;
    builder["email"] = user.email;
    if (user.age) {
        builder["age"] = *user.age;
    }
    builder["role"] = ToString(user.role);
    
    return builder.ExtractValue();
}

} // namespace agromach::models