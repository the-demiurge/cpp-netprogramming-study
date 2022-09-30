#ifndef NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H
#define NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H

#include "common_net.h"
#include "common_protocols.h"
#include "common_utils.h"

#define DEFAULT_PORT 6557

bool handle_mouse_position(SOCKET socket);

WordPacket* process_sentence(MessagePacket* src, WordPacket* dst);

#endif //NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H
