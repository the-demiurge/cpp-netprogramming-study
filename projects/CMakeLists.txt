list(APPEND COMMON_DEPENDENCIES common_net)

if (WIN32)
    LIST(APPEND COMMON_DEPENDENCIES wsock32 ws2_32)
endif()

add_subdirectory(nslookup)
add_subdirectory(tcp)
add_subdirectory(ping)
add_subdirectory(multicast)
