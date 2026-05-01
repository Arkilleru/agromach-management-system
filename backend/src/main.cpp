#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/utils/daemon_run.hpp>

#include "components/storage/tractor_storage.hpp"
#include "handlers/tractors/get.cpp"
#include "handlers/tractors/post_update.cpp"
#include "handlers/tractors/delete.cpp"


int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
        .Append<userver::server::handlers::Ping>()
        .Append<agromach::components::TractorStorage>()
        .Append<agromach::handlers::tractors::GetList>()
        .Append<agromach::handlers::tractors::PostUpdate>()
        .Append<agromach::handlers::tractors::DeleteTractor>();
    return userver::utils::DaemonMain(argc, argv, component_list);
}