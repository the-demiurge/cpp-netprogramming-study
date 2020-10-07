project(simple_ping)

include_directories(include)
file(GLOB SRC src/*.cpp src/*.cxx)
add_executable(${PROJECT_NAME} ${SRC})

list(APPEND HLP_DEPS iphlpapi)

if (MINGW)
 list(APPEND HLP_DEPS icmp)
endif()

target_link_libraries(${PROJECT_NAME} ${COMMON_DEPENDENCIES} ${HLP_DEPS})