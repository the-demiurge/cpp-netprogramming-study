#include "multi_thread_server.h"

THREAD_RESULT handle_connection(void* data) {
	SOCKET socket;
	CHECK_IO((socket = (SOCKET)data) > 0, -1, "Invalid socket\n");
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	CHECK_IO(!getpeername(socket, (sockaddr*)&addr, &addr_len), -1, "Error retrieving peer info\n");
	char* str_in_addr = inet_ntoa(addr.sin_addr);
	printf("[%s:%d]>>%s\n", str_in_addr, ntohs(addr.sin_port), "Establish new connection");
	while (true) {
		char buffer[256] = "";
		int rc = recv(socket, buffer, sizeof(buffer), 0);
		if (rc > 0) {
			printf("[%s:%d]:%s\n", str_in_addr, ntohs(addr.sin_port), buffer);
		}
		else {
			break;
		}
	}
	close_socket(socket);
	printf("[%s:%d]>>%s\n", str_in_addr,ntohs(addr.sin_port), "Close incoming connection");
	return 0;
}