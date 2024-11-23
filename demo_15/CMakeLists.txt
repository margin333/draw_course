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

set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(TMP_GLSL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/fragmentShader.glsl"
                    "${CMAKE_CURRENT_SOURCE_DIR}/vertexShader.glsl")

# 在编译后复制文件和文件夹
add_custom_command(TARGET OpenGLFW POST_BUILD
    # 复制 res 文件夹
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/res" "${CMAKE_CURRENT_BINARY_DIR}/res"
)

# 确保构建目录存在
add_custom_command(TARGET OpenGLFW POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}"
)

# 循环遍历并复制每个 GLSL 文件
foreach(SHADER_FILE ${TMP_GLSL_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    add_custom_command(TARGET OpenGLFW POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SHADER_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}" # 复制到同一目标目录
    )
endforeach()