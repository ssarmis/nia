cmake_minimum_required(VERSION 3.11)

project(nia VERSION 0.0.0 DESCRIPTION "Nia is a cross platform graphics API")

include(GNUInstallDirs)

add_library(nia SHARED 
    ./src/nia_event.cpp
    ./src/arch/linux/nia_window.cpp
    ./src/arch/linux/nia_renderer.cpp)

set_target_properties(nia PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/nia.h)

configure_file(nia.pc.in nia.pc @ONLY)

target_include_directories(nia PRIVATE . ./include/)

install(TARGETS nia
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/nia.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
