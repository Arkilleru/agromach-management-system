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
    
    if (!json.HasMember("id")) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"message\":\"Missing user ID\"}";
    }
    
    std::string id = json["id"].As<std::string>();
    auto user_opt = storage_.GetUserById(id);
    
    if (!user_opt) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return "{\"message\":\"User not found\"}";
    }

    auto user = *user_opt;

    if (json.HasMember("username")) {
        user.name = json["username"].As<std::string>();
    }
    
    if (json.HasMember("email")) {
         std::string new_email = json["email"].As<std::string>();
         
         if (agromach::utils::IsValidEmail(new_email)) {
             auto existing_user = storage_.FindUserByEmail(new_email);
             
             if (existing_user && existing_user->id != id) {
                 request.SetResponseStatus(userver::server::http::HttpStatus::kConflict);
                 return "{\"message\":\"This email is already taken by another user\"}";
             }
             
             user.email = new_email;
         } else {
             request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
             return "{\"message\":\"Invalid email format\"}";
         }
    }
    
    if (json.HasMember("age")) {
        user.age = json["age"].As<int>();
    }

    storage_.UpsertUser(user);

    userver::formats::json::ValueBuilder response;
    response["user"] = user;
    return userver::formats::json::ToString(response.ExtractValue());
}

} // namespace agromach::handlers::users