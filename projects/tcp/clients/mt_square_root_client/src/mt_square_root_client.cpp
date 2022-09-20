#include "mt_square_root_client.h"


THREAD_RESULT send_and_process(void* data) {
    ClientData* client_data = (ClientData*) data;

    SquareRootResponse response;

	int sc = send(client_data->socket, (char*)&(client_data->request), sizeof(SquareRootRequest), 0);
	sc = recv(client_data->socket, (char*)&response, sizeof(SquareRootResponse), 0);

    process_response(&response);

	return 0;
}

void process_response(SquareRootResponse* res) {
    switch (res->status) {
        case TWO_ROOTS:
            printf("Equation roots: x1=%.3f, x2=%.3f\n", res->x1, res->x2);
            break;
        case ONE_ROOTS:
            printf("Equation roots are identical: x1=x2=%.3f\n", res->x1);
            break;
        case NO_ROOTS:
            printf("Equation has no roots\n");
            break;
    }
}