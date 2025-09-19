# CMakeList.txt: projeto de CMake para QtSample, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
cmake_minimum_required(VERSION 3.22)

set(TARGET_LIB_NAME sr2serverlib)
set(TARGET_NAME sr2server)

append_rpath(EXTRA_PATH "${CMAKE_PREFIX_PATH}/lib/x86_64-linux-gnu")
file(GLOB_RECURSE SR2_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE SR2_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SR2_LIB_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "components/*.hpp")
file(GLOB_RECURSE SR2_LIB_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "components/*.cpp")

trace_dependency(NAME zlib INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/zlib_ng.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME zstd INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/zstd.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME brotli INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/brotli.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME lz4 INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/lz4.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME liblzma INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/lzma.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME OpenSSL INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/openssl.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME jsoncpp INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/jsoncpp.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME boost INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/boost.py" COMPONENT_INFIX "_" LIMIT_SEARCH_PATHS)
trace_dependency(NAME Crow INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/crow.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME nghttp2 INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/nghttp2.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME curl INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/libcurl.py" LIMIT_SEARCH_PATHS)
if(WIN32)
	trace_installable_file(NAME libpq INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/postgre_connector.py" LOCATION "${CMAKE_PREFIX_PATH}/bin" EXTENSION "dll" LIMIT_SEARCH_PATHS USE_VSTOOLS)
	trace_installable_file(NAME libmariadb INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/mariadb_connector.py" LOCATION "${CMAKE_PREFIX_PATH}/bin" EXTENSION "dll" LIMIT_SEARCH_PATHS USE_VSTOOLS)
else()
	trace_installable_file(NAME libpq INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/postgre_connector.py" LOCATION "${CMAKE_PREFIX_PATH}/lib/x86_64-linux-gnu" EXTENSION "so" LIMIT_SEARCH_PATHS)
	trace_installable_file(NAME libmariadb INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/mariadb_connector.py" LOCATION "${CMAKE_PREFIX_PATH}/lib" EXTENSION "so" LIMIT_SEARCH_PATHS)
endif()
trace_dependency(NAME Qt6 COMPONENTS Core Sql INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtbase.py" LIMIT_SEARCH_PATHS)
set(Qt6QSQLiteDriverPlugin_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6Sql/")
set(Qt6QMYSQLDriverPlugin_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6Sql/")
set(Qt6QPSQLDriverPlugin_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6Sql/")
trace_dependency(NAME Qt6QSQLiteDriverPlugin INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtsql.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME Qt6QMYSQLDriverPlugin INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtsql.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME Qt6QPSQLDriverPlugin INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtsql.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME range-v3 INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/range3.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME tabulate INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/tabulate.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME TinyOrm INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/tinyorm.py" LIMIT_SEARCH_PATHS)
trace_dependency(NAME flatbuffers INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/flatbuffers.py" LIMIT_SEARCH_PATHS)

if(WIN32)
	set(PYTHON_EXEC python)
else()
	set(PYTHON_EXEC python3)
endif()

file(GLOB_RECURSE FBS_FILES ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}components/fb_schemas/ "*.fbs")
execute_process(COMMAND ${PYTHON_EXEC} ${PROJECT_SOURCE_DIR}/installers/flatc_gen_h.py ${CMAKE_INSTALL_PREFIX} ${CMAKE_CURRENT_SOURCE_DIR}/components/fb_schemas ${FBS_FILES})

add_library(${TARGET_LIB_NAME} SHARED ${SR2_LIB_HDRS} ${SR2_LIB_SRCS})
target_include_directories(${TARGET_LIB_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/commands>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/fb_schemas>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/settings>
	$<BUILD_INTERFACE:${CMAKE_PREFIX_PATH}/include>
)
set_cxx_project_standards(${TARGET_LIB_NAME} 20 FALSE)
target_link_libraries(${TARGET_LIB_NAME} PUBLIC Crow::Crow TinyOrm::TinyOrm)
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