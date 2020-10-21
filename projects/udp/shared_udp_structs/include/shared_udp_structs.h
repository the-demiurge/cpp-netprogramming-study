#ifndef _NETWORK_PROGRAMMING_SHARED_UDP_STRUCTS

#define _NETWORK_PROGRAMMING_SHARED_UDP_STRUCTS

struct MessagePacket {
    char data[256];
};

struct WordPacket {
    char data[256];
    int words;
};
#endif // !_NETWORK_PROGRAMMING_SHARED_UDP_STRUCTS
