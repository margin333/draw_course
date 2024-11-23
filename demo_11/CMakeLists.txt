# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(demo11_OpenGLFW)

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Project_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty
)

# 指定头文件搜索路径
include_directories(${Project_HEADER})
include_directories(header)
include_directories(src)

# 查找源文件
file(GLOB_RECURSE  SRC_SOURCES  "src/*.cpp")

# 查找主程序源文件
set(PEOJECT_MAIN_SRC
    main.cpp
    src/glad.c
)

link_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

# 生成可执行文件，并链接所有源文件
add_executable(OpenGLFW ${PEOJECT_MAIN_SRC} ${HEADER_SOURCES} ${SRC_SOURCES})

target_link_libraries(OpenGLFW glfw3.lib opengl32.lib)