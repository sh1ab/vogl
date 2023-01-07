cmake_minimum_required(VERSION 3.8)

project(sh_vogl_lib VERSION 1.0.0)

file(READ "shaders/big_var.vert.glsl" big_var_vert_shader_text)
file(READ "shaders/big_var.frag.glsl" big_var_frag_shader_text)

configure_file("shaders/shaders.in.hpp" "shaders/shaders.hpp")

include_directories(../sh_ogl)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${PROJECT_NAME}
    STATIC 
        "vogl.hpp"
        "vogl.cpp"
        "math/mat3.hpp"
        "math/vec3.hpp"
        "math/R3_actions.hpp"
        
        "var/var.hpp"

        "var/big_var/big_var_renderer.hpp"
        "var/big_var/big_var_renderer.cpp"

        "shaders/shaders.hpp"
    ) 
    
add_library(sh_libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        "var/big_var/big_var_renderer.hpp"
        "var/var.hpp"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sh_libs::sh_ogl_lib
    )