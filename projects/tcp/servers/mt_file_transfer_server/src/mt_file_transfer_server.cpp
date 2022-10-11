#include "mt_file_transfer_server.h"

THREAD_RESULT handle_connection(void *data) {
    SOCKET socket;
    CHECK_IO((socket = (SOCKET) data) > 0, (THREAD_RESULT) - 1, "Invalid socket\n");
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    CHECK_IO(!getpeername(socket, (sockaddr *) &addr, &addr_len), (THREAD_RESULT) - 1, "Error retrieving peer info\n");
    char *str_in_addr = inet_ntoa(addr.sin_addr);
    printf("[%s:%d]>>%s\n", str_in_addr, ntohs(addr.sin_port), "Establish new connection");
    struct FileHeader file_header;
    memset(&file_header, 0, sizeof(file_header));

    auto rc = recv(socket, (char *) &file_header, sizeof(file_header), 0);

    //CHECK_VOID_IO((rc > 0), "Can't send received file header")
    CHECK_IO((rc > 0), -1, "Can't send received file header")

    struct FileTransferResult result;
    memset(&result, 0, sizeof(result));
    isOK(&file_header, &result);
    //printf("+++ %d", file_header.size);

    rc = send(socket, (char *) &result, sizeof(result), 0);
    //CHECK_VOID_IO((rc > 0), "Can't send result to client")
    CHECK_IO((rc > 0), -1, "Can't send result to client")

    if (FileTransferResult::ACCEPTED != result.status) {
        printf("Can't send %s file\n", file_header.name);
        //return VOID_DEFAULT_RESULT;
        return -1;
    }

    struct FileContent file_content;

    //Download file if it has correct size

    std::ofstream file;
    file.open(file_header.name, std::ios_base::binary);

    long total_received = 0;

    while (total_received != file_header.size) {
        memset(&file_content, 0, sizeof(file_content));
        auto ret = recv(socket, (char*)&file_content, sizeof(file_content), 0);

        file.write(file_content.buffer, file_content.count);
        total_received += file_content.count;
        result.status = FileTransferResult::OK;//My
        ret = send(socket, (char*)&result, sizeof(result), 0);//My
    }


    file.close();
    close_socket(socket);
    printf("[%s]>>%s", str_in_addr, "Close incoming connection");
#ifdef __linux
    return nullptr;
#elif _WIN32
    return 0;
#else
    #error "Not supported platform"
#endif
}

FileTransferResult *isOK(FileHeader *rq, FileTransferResult *rs) {
    if (rq->size > 209715200) {
        rs->status = FileTransferResult::NOT_ACCEPTED_SIZE;
    }
    else {
        rs->status = FileTransferResult::ACCEPTED;
    }
    return rs;
}