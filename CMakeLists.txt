project(TermConnectFour)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(connect_four)
set_property(TARGET connect_four PROPERTY POSITION_INDEPENDENT_CODE ON)

set(SOURCES
	src/main.cpp
)

target_sources(connect_four PRIVATE ${SOURCES})
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(connect_four PRIVATE /W4 /WX)
  target_compile_definitions(connect_four PRIVATE _SCL_SECURE_NO_WARNINGS)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(connect_four PRIVATE -Wall -Wextra -Werror)
endif()
target_link_libraries(connect_four CONAN_PKG::colmc)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
