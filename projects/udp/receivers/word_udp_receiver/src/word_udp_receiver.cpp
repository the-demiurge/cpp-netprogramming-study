#include "word_udp_receiver.h"

bool process_receive_data(SOCKET socket) {
    struct sockaddr_in incom_addr;
    memset(&incom_addr, 0, sizeof(incom_addr));
    unsigned int len = sizeof(incom_addr);
    struct MessagePacket msgPacket {""};
    int cnt = recvfrom(socket, (char*)&msgPacket, sizeof(msgPacket), 0, (sockaddr*)&incom_addr, &len);

    if (cnt <= 0) {
        error_msg("Can't receive data");
    }

    printf("[From: %s] Received data [%s]\n", inet_ntoa(incom_addr.sin_addr), msgPacket.data);

    struct WordPacket wordPacket {"", 0};

    process_sentence(&msgPacket, &wordPacket);

    cnt = sendto(socket, (char*)&wordPacket, sizeof(wordPacket), 0, (sockaddr*)&incom_addr, len);

    if (cnt <= 0) {
        error_msg("Can't send data");
    }

    return true;
}

WordPacket* process_sentence(MessagePacket* src, WordPacket* dst) {
    const char * delims = " ,.-\t";
    char* token = strtok(src->data, delims);
    dst->words = 0;
    while (token) {
        token = strtok(NULL, delims);
        ++(dst->words);
    }

    strcpy(dst->data, src->data);

    return dst;
}