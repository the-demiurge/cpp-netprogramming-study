#include "nslookup.h"

void handle_he_ip(const char* pa, int len)
{
    in_addr addr;
    memcpy(&addr, pa, len);
    printf("%s\n", inet_ntoa(addr));
}