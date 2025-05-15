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

    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/ShaderBase.h
    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/BlinnShader.h
)

set(
    SOURCES
    
    ${CMAKE_SOURCE_DIR}/src/Application.cpp

    ${CMAKE_SOURCE_DIR}/src/RGS/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/WindowsWindow.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Maths.cpp
    ${CMAKE_SOURCE_DIR}/src/RGS/Framebuffer.cpp

    ${CMAKE_SOURCE_DIR}/src/RGS/Shaders/BlinnShader.cpp
)

set(INCULDE_PATH 
    ${CMAKE_SOURCE_DIR}/src
)

set(TARGET RGS)

#set(EXECUTABLE_OUTPUT_PATH output) #设置可执行文件的生成路径

add_executable(
            ${TARGET}

            main.cpp

            ${HEADERS}
            ${SOURCES} 
)

target_include_directories(${TARGET} PRIVATE ${INCLUDE_PATH})

# target_link_libraries(
#             RGS 
#             PRIVATE 
            
#             libglfw3.a
#             libglad.a
# )