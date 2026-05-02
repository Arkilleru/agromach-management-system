#include "auth_checker.hpp"
#include <userver/server/http/http_status.hpp>

namespace agromach::auth {

AuthChecker::AuthChecker(agromach::components::UserStorage& storage, std::string required_permission)
    : storage_(storage), required_permission_(std::move(required_permission)) {}

AuthChecker::AuthCheckResult AuthChecker::CheckAuth(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/) const {
    
    const auto& auth_header = request.GetHeader("Authorization");
    
    if (auth_header.empty()) {
        return AuthCheckResult{
            AuthCheckResult::Status::kInvalidToken,
            "Missing Authorization header"
        };
    }

    static const std::string kBearerPrefix = "Bearer ";
    if (auth_header.size() <= kBearerPrefix.size() || 
        auth_header.compare(0, kBearerPrefix.size(), kBearerPrefix) != 0) {
        return AuthCheckResult{
            AuthCheckResult::Status::kInvalidToken,
            "Invalid Authorization header format (expected Bearer token)"
        };
    }

    std::string token = auth_header.substr(kBearerPrefix.size());
    auto user_opt = storage_.GetUserByToken(token);

    if (!user_opt) {
        return AuthCheckResult{
            AuthCheckResult::Status::kForbidden,
            "Invalid or expired session token"
        };
    }

    if (!required_permission_.empty()) {
        const std::string user_role{agromach::models::ToString(user_opt->role)};
        
        if (user_role != "admin" && user_role != required_permission_) {
            return {
                AuthCheckResult::Status::kForbidden, 
                "Insufficient permissions: " + required_permission_ + " required"
            };
        }
    }

    return {};
}

} // namespace agromach::auth