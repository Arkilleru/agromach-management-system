#include "tractors_handlers.hpp"

namespace agromach::handlers::tractors {

GetTractorList::GetTractorList(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
        storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

std::string GetTractorList::HandleRequestThrow(
    const userver::server::http::HttpRequest&,
    userver::server::request::RequestContext&) const {
    
    const auto all = storage_.GetAllTractors();
    
    userver::formats::json::ValueBuilder builder;
    builder["tractors"] = all;
    
    return userver::formats::json::ToString(builder.ExtractValue());
}
    
} // namespace agromach::handlers::tractors