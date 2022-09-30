#include "mt_file_transfer_server.h"

THREAD_RESULT handle_connection(void *data) {
    SOCKET socket;
    CHECK_IO((socket = *((SOCKET *) data)) > 0, (THREAD_RESULT) - 1, "Invalid socket\n");
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    CHECK_IO(!getpeername(socket, (sockaddr *) &addr, &addr_len), (THREAD_RESULT) - 1, "Error retrieving peer info\n");
    char *str_in_addr = inet_ntoa(addr.sin_addr);
    printf("[%s:%d]>>%s\n", str_in_addr, ntohs(addr.sin_port), "Establish new connection");
    int rc = 1;
    while (rc > 0) {
        FileTransferRequest request;
        memset(&request, sizeof(request), 0);

        FileTransferResponse response;
        memset(&request, sizeof(request), 0);

        rc = recv(socket, (char *) &request, sizeof(request), 0);
        isOK(&request, &response);
        rc = send(socket, (char *) &response, sizeof(response), 0);
        
        //Download file if it has correct size
        if (response.status = OK) {
            
            char file_name[32];
            int file_size = atoi(request.file_size_str);
            char* bytes = new char[file_size];

            std::fstream file;
            file.open(file_name, std::ios_base::out | std::ios_base::binary);

            //Writing file
            if (file.is_open()) {
                file.write(bytes, file_size);
            }   
            delete[] bytes;
            
            //Send response when end of the file is reached
            if (file.eof()) {
                FileTransferResponse final_response;
                memset(&request, sizeof(request), 0);

                final_response.status = SAVED;
                rc = send(socket, (char*)&response, sizeof(response), 0);
            }
            file.close();

        }
        


    }
    close_socket(socket);
    printf("[%s]>>%s", str_in_addr, "Close incomming connection");
    return 0;
#ifdef __linux
    return nullptr;
#endif
}

FileTransferResponse *isOK(FileTransferRequest *rq, FileTransferResponse *rs) {
    int file_size = atoi(rq->file_size_str);
    if (file_size > 209715200) {
        rs->status = BIG_SIZE;
    }
    else {
        rs->status = OK;
    }
    return rs;
}