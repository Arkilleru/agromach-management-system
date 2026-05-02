#include "users_handlers.hpp"

namespace agromach::handlers::users {

PatchUser::PatchUser(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string PatchUser::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    std::string id = json["id"].As<std::string>();

    auto user_opt = storage_.GetUserById(id);
    if (!user_opt) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return "{}";
    }

    auto& user = *user_opt;
    if (json.HasMember("name")) user.name = json["name"].As<std::string>();
    if (json.HasMember("email")) {
         std::string email = json["email"].As<std::string>();
         if (agromach::utils::IsValidEmail(email)) user.email = email;
    }
    if (json.HasMember("age")) user.age = json["age"].As<int>();

    storage_.UpsertUser(user);
    return "{\"status\":\"updated\"}";
}

} // namespace agromach::handlers::users