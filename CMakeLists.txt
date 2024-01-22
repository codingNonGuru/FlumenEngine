cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE None)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a -fpermissive -g -w")

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FlumenEngine)

file(GLOB_RECURSE SOURCE_FILES
	"${PROJECT_SOURCE_DIR}/*"
)

include_directories(/home/andrei/Documents/Code/Libraries/nlohmann)
include_directories(/home/andrei/Documents/Code/Libraries/glm)
include_directories(/home/andrei/Documents/Code/Libraries/FlumenCore)
include_directories(/usr/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DLINUX)

project (Engine)
 
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
#add_executable(Engine ${SOURCE_FILES})

target_link_libraries(Engine GL GLEW pthread SDL2 SDL2_ttf SDL2_image SDL2_mixer)