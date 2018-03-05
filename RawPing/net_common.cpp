#include "net_common.h"
#include <string.h>

int resolve_host(const char* host, sockaddr_in* addr)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	unsigned int laddr = inet_addr(host);
	if (laddr == INADDR_NONE)
	{
		hostent* he = gethostbyname(host);
		if (he == NULL)
		{
			return -1;
		}
		addr->sin_family = he->h_addrtype;
		memcpy(&(addr->sin_addr), *(he->h_addr_list), he->h_length);
		return 0;
	}
	else
	{
		addr->sin_family = AF_INET;
		(addr->sin_addr).s_addr = laddr;
		return 0;
	}
}