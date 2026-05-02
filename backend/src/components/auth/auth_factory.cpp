#include "auth_factory.hpp"

namespace agromach::auth {

userver::server::handlers::auth::AuthCheckerBasePtr AuthCheckerFactory::operator()(
    const userver::components::ComponentContext& context,
    const userver::server::handlers::auth::HandlerAuthConfig& auth_config,
    const userver::server::handlers::auth::AuthCheckerSettings& /*settings*/
) const {
    auto& storage = context.FindComponent<agromach::components::UserStorage>();
    
    auto permission = auth_config["permission"].As<std::string>("");

    return std::make_unique<AuthChecker>(storage, std::move(permission));
}

} // namespace agromach::auth