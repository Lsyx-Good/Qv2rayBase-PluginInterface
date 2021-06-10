cmake_minimum_required(VERSION 3.10.0)
project(QvPluginInterface VERSION 4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Network REQUIRED)

add_library(QvPluginInterface INTERFACE)
add_library(Qv2ray::QvPluginInterface ALIAS QvPluginInterface)

include(GNUInstallDirs)

target_link_libraries(QvPluginInterface INTERFACE Qt::Core Qv2ray::QJsonStruct)

target_include_directories(${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/QvPluginInterface>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set(HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Connections/ConnectionsBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Connections/ConnectionsBaseTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Connections/ConnectionsSafeTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Gui/QvGUIPluginInterface.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/EventHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/KernelHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/OutboundHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/SubscriptionHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Socksify/HttpProxy.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Socksify/SocketStream.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/QvPluginBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/QvPluginInterface.hpp)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/QvPluginInterfaceConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfigVersion.cmake
  COMPATIBILITY ExactVersion)

install(TARGETS QvPluginInterface
    EXPORT QvPluginInterfaceTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT QvPluginInterfaceTargets
    NAMESPACE Qv2ray::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/QvPluginInterfaceMacros.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(EXPORT QvPluginInterfaceTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/QvPluginInterfaceTargets.cmake"
    NAMESPACE Qv2ray::
)
