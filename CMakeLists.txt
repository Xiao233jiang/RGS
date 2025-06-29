cmake_minimum_required(VERSION 3.20)

# 如果未设置构建类型，则默认设置为 Release 模式
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(RGS 
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/

    ${CMAKE_SOURCE_DIR}/3rdlib/
    ${CMAKE_SOURCE_DIR}/3rdlib/imgui/
)

link_directories(
    # ${CMAKE_SOURCE_DIR}/3rdlib/glad/
)

# file(GLOB GLAD_SRC ${CMAKE_SOURCE_DIR}/3rdlib/glad/src/glad.c)

set(
    HEADERS 

    ${CMAKE_SOURCE_DIR}/src/Application.h

    ${CMAKE_SOURCE_DIR}/src/RGS/Base.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Window.h
    ${CMAKE_SOURCE_DIR}/src/RGS/WindowsWindow.h
    ${CMAKE_SOURCE_DIR}/src/RGS/InputCodes.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Maths.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Framebuffer.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Renderer.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Texture.h

    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/ShaderBase.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/BlinnShader.h

    ${CMAKE_SOURCE_DIR}/src/ImGui/ImGuiWindow.h
)

set(
    SOURCES
    
    ${CMAKE_SOURCE_DIR}/src/Application.cpp

    ${CMAKE_SOURCE_DIR}/src/RGS/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/WindowsWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Maths.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Framebuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Texture.cpp

    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/BlinnShader.cpp
    
    ${CMAKE_SOURCE_DIR}/src/stb/stb_image.cpp

    ${CMAKE_SOURCE_DIR}/src/ImGui/imgui_stdlib.cpp
    ${CMAKE_SOURCE_DIR}/src/ImGui/ImGuiWindow.cpp
)

set(INCULDE_PATH 
    ${CMAKE_SOURCE_DIR}/src
)

set(TARGET RGS)

# =========================================
# ================= ImGui =================
# =========================================
file(GLOB IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/3rdlib/imgui/*.cpp)
set(IMGUI_SOURCES ${IMGUI_SOURCES}
    "3rdlib/imgui/backends/imgui_impl_win32.cpp"
    "3rdlib/imgui/backends/imgui_impl_dx11.cpp")
set(IMGUI_LINK_LIBRARY "d3d11.lib" "d3dcompiler.lib" "dxgi.lib" "dwmapi.lib")

add_executable(
            ${TARGET}

            main.cpp

            ${HEADERS}
            ${SOURCES} 

            ${IMGUI_SOURCES}
)

target_include_directories(${TARGET} PRIVATE ${INCLUDE_PATH})

target_link_libraries(
            ${TARGET} 
            PRIVATE 
            
            ${IMGUI_LINK_LIBRARY}
)


set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/assets")                # 设置源资源目录
set(ASSETS_DST "$<TARGET_FILE_DIR:${TARGET}>/assets")       # 设置目标资源目录

set(ASSETS_CMD copy_directory)                           # 设置复制命令为复制目录
set(ASSETS_MSG "Copy Assets")                            # 设置复制信息的提示消息

# 打印复制信息
add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
    "${ASSETS_MSG}: ${ASSETS_SRC} to ${ASSETS_DST}"
)

# 添加自定义命令，在构建目标后复制资源目录
add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E ${ASSETS_CMD}
    "${ASSETS_SRC}" "${ASSETS_DST}"
)