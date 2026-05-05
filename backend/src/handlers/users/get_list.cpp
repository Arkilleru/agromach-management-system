#include "users_handlers.hpp"

namespace agromach::handlers::users {

GetUserList::GetUserList(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string GetUserList::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    const auto& auth_header = request.GetHeader("Authorization");
    
    if (auth_header.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
        return "{\"error\":\"Missing Authorization header\"}";
    }

    std::string_view auth_view = auth_header;
    const std::string_view prefix = "Bearer ";

    if (auth_view.size() <= prefix.size() || 
        auth_view.substr(0, prefix.size()) != prefix) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
        return "{\"error\":\"Invalid authorization type. Bearer token expected.\"}";
    }

    std::string token = std::string(auth_view.substr(prefix.size()));

    auto user_opt = storage_.GetUserByToken(token);

    if (!user_opt || user_opt->role != agromach::models::UserRole::kAdmin) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kForbidden);
        return "{\"error\":\"Access denied: Admin role required\"}";
    }

    const auto users = storage_.GetAllUsers();
    userver::formats::json::ValueBuilder builder;
    builder = users;
    
    return userver::formats::json::ToString(builder.ExtractValue());
}

} // namespace agromach::handlers::users