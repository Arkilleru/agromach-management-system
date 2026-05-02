#include "users_handlers.hpp"

namespace agromach::handlers::users {

PostRegister::PostRegister(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<components::UserStorage>()) {}

std::string PostRegister::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());

    if (!json["name"].IsString() || !json["email"].IsString()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"Missing mandatory fields: name, or email\"}";
    }

    std::string email = json["email"].As<std::string>();
    if (!agromach::utils::IsValidEmail(email)) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"Invalid email format\"}";
    }

    models::User user;
    user.id = userver::utils::generators::GenerateUuid();
    user.name = json["name"].As<std::string>();
    user.email = email;
    
    if (json.HasMember("age") && !json["age"].IsNull()) {
        user.age = json["age"].As<int>();
    }

    user.password_hash = json["password"].As<std::string>("default_hash");
    
    return "{\"status\":\"ok\"}";
}

}