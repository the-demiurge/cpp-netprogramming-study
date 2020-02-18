#include "number_prime_tcp_server.h"

void handle_connection(SOCKET socket, sockaddr_in* addr) {
	char* str_in_addr = inet_ntoa(addr->sin_addr);
	printf("[%s]>>%s\n", str_in_addr, "Establish new connection");
	while (true) {
		int r = 0;
		int rc = recv(socket, (char*)&r, sizeof(r), 0);
		if (rc > 0) {
			printf("[%s]:Received number %d\n", str_in_addr, r);
			char resp[256] = "";
			sprintf(resp, "Number %d is %s", r, is_prime(r) ? "prime" : "compaund");
			rc = send(socket, resp, strlen(resp), 0);
			if (rc <= 0)
			{
				printf("Can't send response to client %s", str_in_addr);
				break;
			}
		}
		else {
			break;
		}
	}
	closesocket(socket);
	printf("[%s]>>%s", str_in_addr, "Close incomming connection");
}

bool is_prime(int n)
{
	for (int i = 2; i < n; ++i)
	{
		if (n % i == 0)
			return false;
	}
	return true;
}