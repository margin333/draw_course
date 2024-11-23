cmake_minimum_required(VERSION 3.12)

project(demo01_OpenGLFW)

set(CMAKE_CXX_STANDARD 17)

set(Project_Source
	fragmentShader.glsl
	vertexShader.glsl
	main.cpp
	glad.c
)

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

add_executable(OpenGLFW ${Project_Source})

target_link_libraries(OpenGLFW glfw3.lib opengl32.lib)