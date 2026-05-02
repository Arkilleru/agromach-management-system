#include "users_handlers.hpp"
#include <userver/formats/json/value_builder.hpp>

namespace agromach::handlers::users {

PostRegister::PostRegister(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<components::UserStorage>()) {}

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

    models::User user;
    user.id = userver::utils::generators::GenerateUuid();
    user.name = json["name"].As<std::string>();
    user.email = email;
    user.password_hash = json["password"].As<std::string>(); 
    user.token = userver::utils::generators::GenerateUuid();
    user.role = models::ParseRole(json["role"].As<std::string>("viewer"));

    if (json.HasMember("age") && !json["age"].IsNull()) {
        user.age = json["age"].As<int>();
    }

    storage_.UpsertUser(user);

    userver::formats::json::ValueBuilder response;
    response["status"] = "ok";
    response["id"] = user.id;
    response["token"] = user.token;
    response["role"] = models::ToString(user.role);

    return userver::formats::json::ToString(response.ExtractValue());
}

} // namespace agromach::handlers::users