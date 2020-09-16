#include "square_root_server.h"

void handle_connection(SOCKET socket, sockaddr_in* addr) {
    char* str_in_addr = inet_ntoa(addr->sin_addr);
    printf("[%s]>>%s\n", str_in_addr, "Establish new connection");
	int rc = 1;
    while (rc > 0) {
        SquareRootRequest request;
		memset(&request, sizeof(request), 0);

		SquareRootResponse response;
		memset(&request, sizeof(request), 0);

        rc = recv(socket, (char*)&request, sizeof(request), 0);
		compute(&request, &response);
		rc = send(socket, (char*)&response, sizeof(response), 0);
    }
    close_socket(socket);
    printf("[%s]>>%s", str_in_addr, "Close incomming connection");
}

SquareRootResponse* compute(SquareRootRequest* rq, SquareRootResponse* rs) {
	double D = rq->b*rq->b - 4 * rq->a*rq->c;
	if (D < 0) {
		rs->status = NO_ROOTS;
	}
	else if (D == 0) {
		rs->status = ONE_ROOTS;
		rs->x1 = rs->x2 = -rq->b / 2 / rq->b;
	}
	else {
		rs->status = TWO_ROOTS;
		rs->x1 = (-rq->b - sqrt(D)) / 2 / rq->b;
		rs->x2 = (-rq->b + sqrt(D)) / 2 / rq->b;
	}

	return rs;
}