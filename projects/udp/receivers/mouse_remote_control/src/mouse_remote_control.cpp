#include "mouse_remote_control.h"

bool handle_mouse_position(SOCKET socket) {
    struct sockaddr_in incom_addr;
    memset(&incom_addr, 0, sizeof(incom_addr));

    socklen_t len = sizeof(incom_addr);

    MOUSE_POSITION mouse_position {0, 0};

    int ret = recvfrom(socket, (char*)&mouse_position, sizeof(mouse_position), 0, (sockaddr*)&incom_addr, &len);

    if (ret <= 0) {
        error_msg("Can't receive data");
    } else {
        printf("[From: %s] Mouse position [(%d, %d)]\n", inet_ntoa(incom_addr.sin_addr), mouse_position.x, mouse_position.y);
    }

    return true;
}