#include "udp_echo_receiver.h"

bool process_receiv_data(SOCKET socket) {
	sockaddr_in incom_addr;
	memset(&incom_addr, 0, sizeof(incom_addr));
	socklen_t len = sizeof(incom_addr);
	char buffer[256] = "";
	int rec_cn = recvfrom(socket, buffer, sizeof(buffer), 0, (sockaddr*)&incom_addr, &len);

	if (rec_cn <= 0) {
		error_msg("Can't receive data");		
	}

	printf("[From: %s] Received message [%s]\n", inet_ntoa(incom_addr.sin_addr), buffer);
	int sc = sendto(socket, buffer, sizeof(buffer), 0, (sockaddr*)&incom_addr, sizeof(incom_addr));
	printf("[To: %s] Sending echoed message [%s]\n", inet_ntoa(incom_addr.sin_addr), buffer);
	return true;
}