#ifndef NETWORK_PROGRAMMING_MOUSE_REMOTE_MANAGER_H
#define NETWORK_PROGRAMMING_MOUSE_REMOTE_MANAGER_H

#include "common_net.h"
#include "common_protocols.h"
#include "common_utils.h"
#include "string.h"
#include "stdio.h"

#define DEFAULT_REMOTE_CONTROL_HOST "127.0.0.1"
#define DEFAULT_REMOTE_CONTROL_PORT 6557

bool is_mouse_moved(PMOUSE_POSITION prev, PMOUSE_POSITION current);

#endif //NETWORK_PROGRAMMING_MOUSE_REMOTE_MANAGER_H
