#include "users_handlers.hpp"

namespace agromach::handlers::users {

GetUserById::GetUserById(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string GetUserById::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    const auto& id = request.GetArg("id");
    if (id.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"id parameter is required\"}";
    }

    auto user = storage_.GetUserById(id);

    if (!user) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return "{\"error\":\"user not found\"}";
    }

    return userver::formats::json::ToString(
        userver::formats::json::ValueBuilder{*user}.ExtractValue()
    );
}

}