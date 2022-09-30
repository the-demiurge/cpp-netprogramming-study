#include "mt_file_transfer_client.h"


THREAD_RESULT send_and_process(void* data) {
    ClientData* client_data = (ClientData*) data;

    FileTransferResponse response;

	int sc = send(client_data->socket, (char*)&(client_data->request), sizeof(FileTransferRequest), 0);
	sc = recv(client_data->socket, (char*)&response, sizeof(FileTransferResponse), 0);

    process_response(&response);

	return 0;
}

void process_response(FileTransferResponse* res) {
    switch (res->status) {
        case OK:
            printf("Transferring the file...\n");
            break;
        case BIG_SIZE:
            printf("Your file is too big.\n");
            break;
        case SAVED:
            printf("Your file saved to server successfully.\n");
            break;
    }
}