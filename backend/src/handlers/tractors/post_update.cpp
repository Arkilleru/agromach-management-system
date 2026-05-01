#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component_context.hpp>

#include "../../components/storage/tractor_storage.hpp"

namespace agromach::handlers::tractors {

class PostUpdate final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractor-post";

    PostUpdate(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& context)
        : HttpHandlerBase(config, context),
          storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext&) const override {
        
        auto json = userver::formats::json::FromString(request.RequestBody());
        auto tractor = json.As<models::Tractor>();
        
        storage_.UpdateTractor(std::move(tractor));
        
        return "{\"status\":\"ok\"}";
    }

private:
    agromach::components::TractorStorage& storage_;
};

}