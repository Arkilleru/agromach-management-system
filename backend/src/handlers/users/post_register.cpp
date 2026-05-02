#include "users_handlers.hpp"

namespace agromach::handlers::users {

PostRegister::PostRegister(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string PostRegister::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());

    if (!json["name"].IsString() || !json["email"].IsString() || !json["password"].IsString()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"Missing mandatory fields: name, email, or password\"}";
    }

    std::string email = json["email"].As<std::string>();
    
    if (storage_.FindUserByEmail(email).has_value()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
        return "{\"error\":\"User with this email already exists\"}";
    }

    agromach::models::User user;
    user.id = userver::utils::generators::GenerateUuid();
    user.name = json["name"].As<std::string>();
    user.email = email;
    
    std::string raw_password = json["password"].As<std::string>();
    user.password_hash = agromach::utils::HashPassword(raw_password); 

    user.token = userver::utils::generators::GenerateUuid();
    user.role = agromach::models::ParseRole(json["role"].As<std::string>("viewer"));

    if (json.HasMember("age") && !json["age"].IsNull()) {
        user.age = json["age"].As<int>();
    }

    storage_.UpsertUser(user);

    userver::formats::json::ValueBuilder response;
    response["status"] = "ok";
    response["id"] = user.id;
    response["token"] = user.token;
    response["role"] = agromach::models::ToString(user.role);

    return userver::formats::json::ToString(response.ExtractValue());
}

} // namespace agromach::handlers::users