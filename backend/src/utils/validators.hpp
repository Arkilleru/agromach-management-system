#pragma once

#include <userver/crypto/hash.hpp>
#include <string>
#include <regex>

namespace agromach::utils {

inline bool IsValidEmail(const std::string& email) {
    static const std::regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    return std::regex_match(email, pattern);
}


inline std::string HashPassword(const std::string& password) {
    return userver::crypto::hash::Sha256(password + "agromach_salt_2026");
}

} // namespace agromach::utils