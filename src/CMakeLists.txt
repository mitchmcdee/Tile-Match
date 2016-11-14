set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES main.cpp)
add_executable(tile_match ${SOURCE_FILES})

#Find and add SFML
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
if(WIN32)
	find_package(SFML 2 REQUIRED COMPONENTS audio graphics network system window main)
else()
	find_package(SFML 2 REQUIRED COMPONENTS audio graphics network system window)
endif()
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(tile_match ${SFML_LIBRARIES})
endif()
