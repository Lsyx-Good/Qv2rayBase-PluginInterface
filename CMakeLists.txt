cmake_minimum_required(VERSION 3.10.0)

project(QvPluginInterface VERSION 4.0)

find_package(Qt6 COMPONENTS Core REQUIRED)

set(INTERFACE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Common/CommonTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Common/CommonSafeType.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Common/QvPluginBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Connections/ConnectionsBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Utils/BindableProps.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Utils/QJsonIO.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Utils/JsonConversion.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Utils/ForEachMacros.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/QvPluginInterface.hpp
)

set(FEATURE_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Gui/QvGUIPluginInterface.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/EventHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/IProfilePreprocessor.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/KernelHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/OutboundHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/SubscriptionHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Handlers/LatencyTestHandler.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Socksify/HttpProxy.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/Socksify/SocketStream.hpp
)

add_library(QvPluginInterface INTERFACE ${INTERFACE_HEADERS} ${FEATURE_HEADERS})
add_library(Qv2ray::QvPluginInterface ALIAS QvPluginInterface)

set_target_properties(QvPluginInterface PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
target_link_libraries(QvPluginInterface INTERFACE Qt::Core)

include(GNUInstallDirs)

#foreach(HEADER ${INTERFACE_HEADERS})
#    target_sources(QvPluginInterface INTERFACE
#        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface/${HEADER}>"
#        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${HEADER}>")
#endforeach()

target_include_directories(${PROJECT_NAME} INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/QvPluginInterface>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/QvPluginInterfaceConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(TARGETS QvPluginInterface
    EXPORT QvPluginInterfaceTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT QvPluginInterfaceTargets
    NAMESPACE Qv2ray::
    COMPONENT "development"
    EXCLUDE_FROM_ALL
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QvPluginInterfaceConfig.cmake
    COMPONENT "development"
    EXCLUDE_FROM_ALL
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/QvPluginInterfaceMacros.cmake
    COMPONENT "development"
    EXCLUDE_FROM_ALL
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QvPluginInterface)

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    COMPONENT "development"
    EXCLUDE_FROM_ALL
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(EXPORT QvPluginInterfaceTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/QvPluginInterfaceTargets.cmake"
    NAMESPACE Qv2ray::
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/QvPluginInterfaceMacros.cmake)
