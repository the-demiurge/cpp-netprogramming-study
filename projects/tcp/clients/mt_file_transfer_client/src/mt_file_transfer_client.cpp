#include "mt_file_transfer_client.h"


THREAD_RESULT send_and_process(void* data) {
    ClientHeaderData* client_data = (ClientHeaderData*) data;

    FileTransferResult response;

	int sc = send(client_data->socket, (char*)&(client_data->request), sizeof(FileHeader), 0);
	sc = recv(client_data->socket, (char*)&response, sizeof(FileTransferResult), 0);

    process_response(&response);

	return 0;
}

void process_response(FileTransferResult* res) {
    switch (res->status) {
        case FileTransferResult::ACCEPTED:
            printf("Transferring the file...\n");
            break;
        case FileTransferResult::NOT_ACCEPTED_SIZE:
            printf("Your file is too big.\n");
            break;
        case FileTransferResult::OK:
            printf("Your file saved to server successfully.\n");
            break;
        case FileTransferResult::FAIL:
            printf("Your file saved to server failure.\n");
            break;
    }
}