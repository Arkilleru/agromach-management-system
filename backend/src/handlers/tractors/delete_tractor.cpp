#include "tractors_handlers.hpp"

namespace agromach::handlers::tractors {

DeleteTractor::DeleteTractor(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
        storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

std::string DeleteTractor::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    const auto& id = request.GetArg("id");
    if (id.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"missing id\"}";
    }

    if (storage_.RemoveTractor(id)) {
        return "{\"status\":\"deleted\"}";
    }
    
    request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
    return "{\"status\":\"not_found\"}";
}

} // namespace agromach::handlers::tractors