#include "tractors_handlers.hpp"
#include <userver/utils/uuid4.hpp>

namespace agromach::handlers::tractors {

PostRegister::PostRegister(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
        storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

std::string PostRegister::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
    
    try {
        auto json = userver::formats::json::FromString(request.RequestBody());
        auto tractor = json.As<models::Tractor>();
        tractor.id = userver::utils::generators::GenerateUuid();

        storage_.UpsertTractor(std::move(tractor));

        return "{\"status\":\"ok\"}";
        
    } catch (const userver::formats::json::Exception& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        return userver::formats::json::ToString(
            userver::formats::json::MakeObject(
                "error", "Invalid request body",
                "details", e.what()
            )
        );
    } catch (const std::exception& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kInternalServerError);
        return "{\"error\":\"Internal server error\"}";
    }
}

} // namespace agromach::handlers::tractors