#include "tractors_handlers.hpp"

namespace agromach::handlers::tractors {

PostRegister::PostRegister(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
        storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

std::string PostRegister::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    auto json = userver::formats::json::FromString(request.RequestBody());
    auto tractor = json.As<models::Tractor>();
    
    storage_.UpsertTractor(std::move(tractor));
    
    return "{\"status\":\"ok\"}";
}

} // namespace agromach::handlers::tractors