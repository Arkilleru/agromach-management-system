#pragma once

#include <userver/server/handlers/auth/auth_checker_factory.hpp>
#include "auth_checker.hpp"
#include "../../components/storage/user_storage.hpp"

namespace agromach::auth {

class AuthCheckerFactory final : public userver::server::handlers::auth::AuthCheckerFactoryBase {
public:
    static constexpr std::string_view kAuthType = "bearer";

    userver::server::handlers::auth::AuthCheckerBasePtr operator()(
        const userver::components::ComponentContext& context,
        const userver::server::handlers::auth::HandlerAuthConfig& auth_config,
        const userver::server::handlers::auth::AuthCheckerSettings& settings
    ) const override;
};

} // namespace agromach::auth