#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/formats/json/value_builder.hpp> 
#include <userver/formats/json/exception.hpp>
#include <userver/formats/serialize/common_containers.hpp>

#include "../../components/storage/tractor_storage.hpp"
#include "../../utils/validators.hpp"

namespace agromach::handlers::tractors {

class PostRegister final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractor-post";
    
    PostRegister(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::TractorStorage& storage_;
};

class GetTractorList final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractors-list";
    
    GetTractorList(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::TractorStorage& storage_;
};

class GetTractorById final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-get-tractor";
    
    GetTractorById(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::TractorStorage& storage_;
};

class DeleteTractor final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-tractor-delete";
    
    DeleteTractor(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::TractorStorage& storage_;
};

} // namespace agromach::handlers::tractors