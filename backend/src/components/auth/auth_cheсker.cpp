#include "auth_checker.hpp"
#include <userver/server/http/http_status.hpp>

namespace agromach::auth {

AuthChecker::AuthChecker(agromach::components::UserStorage& storage, std::string required_permission)
    : storage_(storage), required_permission_(std::move(required_permission)) {}

AuthChecker::AuthCheckResult AuthChecker::CheckAuth(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/) const {
    
    const auto& api_key = request.GetHeader("X-API-Key");
    if (api_key.empty()) {
        return AuthCheckResult{
            AuthCheckResult::Status::kInvalidToken,
            "Missing X-API-Key header"
        };
    }

    auto user_opt = storage_.GetUserByToken(api_key);

    if (!user_opt) {
        return AuthCheckResult{
            AuthCheckResult::Status::kForbidden,
            "Invalid API Key"
        };
    }

    if (!required_permission_.empty()) {
        if (agromach::models::ToString(user_opt->role) != required_permission_) {
            return {AuthCheckResult::Status::kForbidden, "Insufficient permissions"};
        }
    }

    return {}; // Успех
}

} // namespace agromach::auth