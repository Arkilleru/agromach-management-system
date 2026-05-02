#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/server/handlers/auth/auth_checker_settings_component.hpp>
#include <userver/server/handlers/auth/auth_checker_base.hpp>
#include <userver/server/handlers/handler_config.hpp>



#include "components/auth/auth_factory.hpp"
#include "components/auth/auth_checker.hpp"

#include "components/storage/tractor_storage.hpp"
#include "components/storage/user_storage.hpp" 

#include "handlers/tractors/tractors_handlers.hpp"
#include "handlers/users/users_handlers.hpp"

int main(int argc, char* argv[]) {
    userver::server::handlers::auth::RegisterAuthCheckerFactory(
        std::string(agromach::auth::AuthCheckerFactory::kAuthType),
        std::make_unique<agromach::auth::AuthCheckerFactory>()
    );

    auto component_list = userver::components::MinimalServerComponentList()
        .Append<userver::server::handlers::Ping>()
        
        .Append<agromach::components::TractorStorage>()
        .Append<agromach::components::UserStorage>() 
        
        .Append<agromach::handlers::tractors::GetTractorById>()
        .Append<agromach::handlers::tractors::GetTractorList>()
        .Append<agromach::handlers::tractors::PostRegister>()
        .Append<agromach::handlers::tractors::DeleteTractor>()

        .Append<agromach::handlers::users::PostRegister>()
        .Append<agromach::handlers::users::GetUserList>()
        .Append<agromach::handlers::users::GetUserById>()
        .Append<agromach::handlers::users::PatchUser>()
        .Append<agromach::handlers::users::DeleteUser>()
        .Append<agromach::handlers::users::Login>()

        .Append<userver::components::AuthCheckerSettings>("my-auth-settings");


    return userver::utils::DaemonMain(argc, argv, component_list);
}