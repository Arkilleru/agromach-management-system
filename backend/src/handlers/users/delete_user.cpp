#include "users_handlers.hpp"

namespace agromach::handlers::users {

DeleteUser::DeleteUser(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string DeleteUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    const auto& admin_token = request.GetHeader("X-Admin-Token");
    if (admin_token != "super-secret-admin-key") {
        request.SetResponseStatus(userver::server::http::HttpStatus::kForbidden);
        return "{\"error\":\"Only admins can delete users\"}";
    }

    const auto& id = request.GetArg("id");
    if (id.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"Missing id parameter\"}";
    }

    bool removed = storage_.RemoveUser(id);

    if (!removed) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return "{\"error\":\"User not found\"}";
    }

    return "{\"status\":\"deleted\"}";
}

}