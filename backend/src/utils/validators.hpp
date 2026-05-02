#pragma once
#include <string>
#include <regex>

namespace agromach::utils {

inline bool IsValidEmail(const std::string& email) {
    static const std::regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    return std::regex_match(email, pattern);
}

} // namespace agromach::utils