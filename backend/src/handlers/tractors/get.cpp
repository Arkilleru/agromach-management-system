#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "../../components/storage/tractor_storage.hpp"

namespace agromach::handlers::tractors {

class GetList final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractor-get";

    GetList(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& context)
        : HttpHandlerBase(config, context),
          storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest&,
        userver::server::request::RequestContext&) const override {
        
        const auto all = storage_.GetAllTractors();
        
        userver::formats::json::ValueBuilder builder;
        builder = all;
        
        return userver::formats::json::ToString(builder.ExtractValue());
    }

private:
    agromach::components::TractorStorage& storage_;
};
    
}