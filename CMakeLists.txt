cmake_minimum_required(VERSION 3.11)

project(nia VERSION 0.0.0 DESCRIPTION "Nia is a cross platform graphics API")

include(GNUInstallDirs)

add_definitions(-DNIA_LIBRARY_BUILD -DNIA_RENDERER_GL -DGL_GLEXT_PROTOTYPES -DNIA_DEBUG_BUILD)

if (UNIX)
    add_library(nia SHARED 
        ./src/nia_shader.cpp
        ./src/nia_event.cpp
        ./src/nia_wgl.cpp
        ./src/nia_gl.cpp
        ./src/nia_batch_renderer_gl.cpp
        ./src/arch/linux/nia_window.cpp
        ./src/arch/linux/nia_renderer.cpp)
endif (UNIX)

if (WIN32)
    add_library(nia SHARED 
        ./src/nia_shader.cpp
        ./src/nia_event.cpp
        ./src/nia_wgl.cpp
        ./src/nia_gl.cpp
        ./src/nia_batch_renderer_gl.cpp
        ./src/arch/win32/nia_window_gl.cpp
        ./src/arch/win32/nia_renderer_gl.cpp)
endif (WIN32)

set_target_properties(nia PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/nia.h)

configure_file(nia.pc.in nia.pc @ONLY)

target_include_directories(nia PRIVATE . ./include/)

if (WIN32)
    target_link_libraries(nia opengl32 gdi32)
endif (WIN32)

install(TARGETS nia
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/nia.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
