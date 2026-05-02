#include "tractors_handlers.hpp"

namespace agromach::handlers::tractors {

GetTractorById::GetTractorById(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
        storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

std::string GetTractorById::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    const auto& id = request.GetArg("id");
    if (id.empty()) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return "{\"error\":\"Missing id parameter\"}";
    }

    auto tractor = storage_.GetTractorById(id);

    if (!tractor) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return "{\"error\":\"Tractor not found\"}";
    }

    userver::formats::json::ValueBuilder builder{*tractor};
    return userver::formats::json::ToString(builder.ExtractValue());
}

} // namespace agromach::handlers::tractors