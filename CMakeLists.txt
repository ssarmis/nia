cmake_minimum_required(VERSION 3.11)

project(nia VERSION 0.0.0 DESCRIPTION "Nia is a cross platform graphics API")

include(GNUInstallDirs)

add_definitions(-DNIA_LIBRARY_BUILD -DNIA_RENDERER_GL -DGL_GLEXT_PROTOTYPES -msse2)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DNIA_DEBUG_BUILD)
endif(CMAKE_BUILD_TYPE MATCHES "Debug")

if (UNIX)
    add_library(nia SHARED 
        ./src/nia_array.cpp
        ./src/nia_matrix.cpp
        ./src/nia_vector.cpp
        ./src/parsers/nia_ttf_parser.cpp
        ./src/parsers/nia_bmp_parser.cpp
        ./src/parsers/nia_tga_parser.cpp
        ./src/parsers/nia_obj_parser.cpp
        ./src/nia_transform.cpp
        ./src/nia_texture_streaming.cpp
        ./src/nia_loader.cpp
        ./src/shaders/nia_shader.cpp
        ./src/shaders/nia_shader_batch.cpp
        ./src/shaders/nia_shader_font.cpp
        ./src/shaders/nia_shader_filter.cpp
        ./src/shaders/nia_shader_cubemap.cpp
        ./src/shaders/nia_shader_reflective.cpp
        ./src/shaders/nia_shader_quad.cpp
        ./src/nia_cube_texture.cpp
        ./src/nia_texture_gl.cpp
        ./src/nia_frame_buffer.cpp
        ./src/nia_filter.cpp
        ./src/nia_post_processing_pipeline.cpp
        ./src/nia_event.cpp
        ./src/nia_wgl.cpp
        ./src/nia_gl.cpp
        ./src/nia_mesh.cpp
        ./src/nia_scene.cpp
        ./src/nia_renderer_gl.cpp
        ./src/nia_batch_renderer_gl.cpp
        ./src/nia_font_renderer_gl.cpp
        ./src/arch/linux/nia_window.cpp
        ./src/arch/linux/nia_renderer.cpp)
endif (UNIX)

if (WIN32)
    add_library(nia SHARED 
        ./src/nia_array.cpp
        ./src/nia_matrix.cpp
        ./src/nia_vector.cpp
        ./src/parsers/nia_ttf_parser.cpp
        ./src/parsers/nia_obj_parser.cpp
        ./src/parsers/nia_bmp_parser.cpp
        ./src/parsers/nia_tga_parser.cpp
        ./src/nia_transform.cpp
        ./src/nia_texture_streaming.cpp
        ./src/nia_loader.cpp
        ./src/shaders/nia_shader.cpp
        ./src/shaders/nia_shader_batch.cpp
        ./src/shaders/nia_shader_font.cpp
        ./src/shaders/nia_shader_filter.cpp
        ./src/shaders/nia_shader_cubemap.cpp
        ./src/shaders/nia_shader_reflective.cpp
        ./src/shaders/nia_shader_quad.cpp
        ./src/nia_cube_texture.cpp
        ./src/nia_texture_gl.cpp
        ./src/nia_frame_buffer.cpp
        ./src/nia_filter.cpp
        ./src/nia_post_processing_pipeline.cpp
        ./src/nia_event.cpp
        ./src/nia_wgl.cpp
        ./src/nia_gl.cpp
        ./src/nia_mesh.cpp
        ./src/nia_scene.cpp
        ./src/nia_renderer_gl.cpp
        ./src/nia_batch_renderer_gl.cpp
        ./src/nia_font_renderer_gl.cpp
        ./src/arch/win32/nia_window_gl.cpp)
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
