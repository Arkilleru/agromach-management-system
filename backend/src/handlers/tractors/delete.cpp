#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component_context.hpp>

#include "../../components/storage/tractor_storage.hpp"

namespace agromach::handlers::tractors {

class DeleteTractor final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractor-delete";

    DeleteTractor(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context)
        : HttpHandlerBase(config, context),
          storage_(context.FindComponent<agromach::components::TractorStorage>()) {}

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext&) const override {
        
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

private:
    agromach::components::TractorStorage& storage_;
};

}