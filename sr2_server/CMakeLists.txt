# CMakeList.txt: projeto de CMake para QtSample, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
cmake_minimum_required(VERSION 3.22)

set(TARGET_LIB_NAME sr2serverlib)
set(TARGET_NAME sr2server)

append_rpath()
file(GLOB_RECURSE SR2_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE SR2_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SR2_LIB_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "components/*.hpp")
file(GLOB_RECURSE SR2_LIB_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "components/*.cpp")

trace_dependency(NAME boost INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/boost.py" COMPONENT_INFIX "_" LIMIT_SEARCH_PATHS)
trace_dependency(NAME Crow INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/crow.py" LIMIT_SEARCH_PATHS)

add_library(${TARGET_LIB_NAME} SHARED ${SR2_LIB_HDRS} ${SR2_LIB_SRCS})
target_include_directories(${TARGET_LIB_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components>
	$<BUILD_INTERFACE:${CMAKE_PREFIX_PATH}/include>
)
set_cxx_project_standards(${TARGET_LIB_NAME} 20 FALSE)
target_link_libraries(${TARGET_LIB_NAME} PUBLIC Crow::Crow)
if(WIN32)
	target_compile_definitions(${TARGET_LIB_NAME} PRIVATE LIB_BUILD)
	target_compile_definitions(${TARGET_LIB_NAME} INTERFACE LIB_EXPORT)
endif()

add_executable(${TARGET_NAME} ${SR2_HDRS} ${SR2_SRCS})
target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_LIB_NAME})
target_include_directories(${TARGET_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set_cxx_project_standards(${TARGET_NAME} 20 FALSE)