cmake_minimum_required(VERSION 3.12...3.31)
project(agromach_backend CXX) 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)


find_package(userver COMPONENTS core QUIET)
if(NOT userver_FOUND)
  download_userver(TRY_DIR third_party/userver VERSION 2.8)
endif()

userver_setup_environment()

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE userver::core)

include(GNUInstallDirs)
file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME})