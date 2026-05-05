#include "users_handlers.hpp"
#include <userver/utils/uuid4.hpp>

namespace agromach::handlers::users {

Login::Login(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string Login::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto email = json["email"].As<std::string>();
    auto password = json["password"].As<std::string>();

    auto user_opt = storage_.FindUserByEmail(email);
    
    if (!user_opt) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
        return "{\"message\": \"Неверный логин или пароль\"}";
    }

    std::string input_hash = agromach::utils::HashPassword(password);
    
    if (user_opt->password_hash != input_hash) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kUnauthorized);
        return "{\"message\": \"Неверный логин или пароль\"}";
    }
    
    std::string token = user_opt->token;
    userver::formats::json::ValueBuilder response;
    response["token"] = token;
    
    response["user"] = *user_opt;

    return userver::formats::json::ToString(response.ExtractValue());
}

} // namespace agromach::handlers::users