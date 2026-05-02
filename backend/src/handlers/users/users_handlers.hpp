#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/formats/json/value_builder.hpp> 
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utils/uuid4.hpp>

#include "../../components/storage/user_storage.hpp"
#include "../../utils/validators.hpp"

namespace agromach::handlers::users {

class PostRegister final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-user-register";
    
    PostRegister(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::UserStorage& storage_;
};

class GetUserList final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-users-list";
    
    GetUserList(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::UserStorage& storage_;
};

class GetUserById final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-get-user";
    
    GetUserById(const userver::components::ComponentConfig& config,
                const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::UserStorage& storage_;
};

class PatchUser final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-patch-user";
    
    PatchUser(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::UserStorage& storage_;
};

class DeleteUser final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-delete-user";
    
    DeleteUser(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest& request, 
                                   userver::server::request::RequestContext& context) const override;
private:
    agromach::components::UserStorage& storage_;
};

} // namespace agromach::handlers::users