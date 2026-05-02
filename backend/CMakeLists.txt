cmake_minimum_required(VERSION 3.12...3.31)
project(agromach_backend CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(DownloadUserver)
download_userver(TRY_DIR third_party/userver VERSION 2.6)

userver_setup_environment()

add_executable(${PROJECT_NAME} 
    src/main.cpp
  
    src/handlers/tractors/get_tractor.cpp
    src/handlers/tractors/get_list.cpp
    src/handlers/tractors/post_update.cpp
    src/handlers/tractors/delete_tractor.cpp

    src/handlers/users/get_list.cpp
    src/handlers/users/get_user.cpp
    src/handlers/users/post_register.cpp
    src/handlers/users/patch_user.cpp
    src/handlers/users/delete_user.cpp

    src/components/auth/auth_factory.cpp
    src/components/auth/auth_cheсker.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    userver::core 
    userver::universal
)

include(GNUInstallDirs)
file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME})