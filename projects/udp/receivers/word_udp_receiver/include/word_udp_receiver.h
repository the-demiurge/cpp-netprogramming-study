#ifndef NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H
#define NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H

#include "common_net.h"
#include "shared_udp_structs.h"

#define DEFAULT_PORT 6557

bool process_receive_data(SOCKET socket);

WordPacket* process_sentence(MessagePacket* src, WordPacket* dst);

#endif //NETWORK_PROGRAMMING_WORD_UDP_UDP_RECEIVER_H
