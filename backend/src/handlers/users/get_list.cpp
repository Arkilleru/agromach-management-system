#include "users_handlers.hpp"

namespace agromach::handlers::users {

GetUserList::GetUserList(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<agromach::components::UserStorage>()) {}

std::string GetUserList::HandleRequestThrow(
    const userver::server::http::HttpRequest&,
    userver::server::request::RequestContext&) const {
    
    const auto users = storage_.GetAllUsers();
    userver::formats::json::ValueBuilder builder;
    builder = users;
    
    return userver::formats::json::ToString(builder.ExtractValue());
}

}