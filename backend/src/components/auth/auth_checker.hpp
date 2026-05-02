#pragma once

#include <userver/server/handlers/auth/auth_checker_base.hpp>
#include <string>
#include "../../components/storage/user_storage.hpp"

namespace agromach::auth {

class AuthChecker final : public userver::server::handlers::auth::AuthCheckerBase {
public:
    using AuthCheckResult = userver::server::handlers::auth::AuthCheckResult;

    AuthChecker(agromach::components::UserStorage& storage, std::string required_permission);

    AuthCheckResult CheckAuth(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;

    bool SupportsUserAuth() const noexcept override { return true; }

private:
    agromach::components::UserStorage& storage_;
    std::string required_permission_;
};

} // namespace agromach::auth