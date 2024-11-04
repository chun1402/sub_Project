cmake_minimum_required(VERSION 3.10)
project(axisRobot)

# C++ 표준 설정
set (CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenGL 및 GLFW 추가 (필요 시)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

# ImGui 디렉토리 설정
set(IMGUI "${PROJECT_SOURCE_DIR}/../imgui")
list(APPEND INC_DIRS ${IMGUI} ${IMGUI}/backends)
file(GLOB IMGUISRC "${IMGUI}/*.cpp")

include_directories( 
	${CMAKE_SOURCE_DIR}/src
	/usr/include
	${INC_DIRS}
)

add_library(imgui SHARED
	${IMGUISRC}
	${IMGUI}/backends/imgui_impl_opengl3.cpp
	${IMGUI}/backends/imgui_impl_glfw.cpp
	${IMGUI}/misc/cpp/imgui_stdlib.cpp
)

TARGET_LINK_LIBRARIES(imgui glfw)

#src	
file(GLOB SRCs "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 프로젝트 소스 파일 추가
add_executable(${PROJECT_NAME}
    ./apps/main.cpp
    ${IMGUI_SOURCES}
	 ${SRCs}
)

# 라이브러리 링크
target_link_libraries(${PROJECT_NAME} 
	imgui
	OpenGL 
	glfw
	GLEW::GLEW GL
)

