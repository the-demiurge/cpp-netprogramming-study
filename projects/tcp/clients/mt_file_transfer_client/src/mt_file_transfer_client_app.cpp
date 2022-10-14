#include "mt_file_transfer_client.h"

void exit_handler();

SOCKET client_socket = 0;


int main(int argc, char* argv[])
{
	COMMAND_OPTIONS cmd_opts{ DEFAULT_SERVER_HOST, DEFAULT_SERVER_PORT};

	parse_cmd(argc, argv, &cmd_opts);

	if (!valid_connection_opts(&cmd_opts))
	{
		common_usage(argv[0]);
		return -1;
	}

	atexit(common_exit_handler);
	atexit(exit_handler);

	common_init_handler();

	CHECK_IO((client_socket = create_tcp_socket()) > 0, -1, "Error create client socket");

	struct sockaddr_in server_addr;
	init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);
	//Connect to the server
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr))) {
		char err_msg[512] = "";
		sprintf(err_msg, "Can't connect to the server %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

	printf("Connection to the server %s:%d success\n", cmd_opts.host, cmd_opts.port);

    struct FileHeader file_header;
    memset(&file_header, 0, sizeof(file_header));

    {
        char file_name[FILE_HEADER_SIZE];
        printf("Type a file name:");
        scanf("%s", file_name);

        std::filesystem::path file_path{file_name};

        if (!std::filesystem::exists(file_path)) {
            printf("Sorry but file [%s] doesn't exist", file_path.c_str());
            return -1;
        }

        strcpy(file_header.name, file_name);
        file_header.size = std::filesystem::file_size(file_path);
    }

    //Send file header to server
    auto ret = send(client_socket, (char*)&file_header, sizeof(FileHeader), 0);

    CHECK_IO((send(client_socket, (char*)&file_header, sizeof(FileHeader), 0) > 0), -1, "Error to send file header");

    struct FileTransferResult result;

    memset(&result, 0, sizeof(result));

    ret = recv(client_socket, (char*)&result, sizeof(FileTransferResult), 0);

    CHECK_IO((recv(client_socket, (char*)&result, sizeof(FileTransferResult), 0) > 0), -1, "Error to received server response");

    CHECK_IO((result.status != FileTransferResult::ACCEPTED), -1, "File size is not accepted");


    //UPLOAD FILE
	std::fstream file;
	file.open(file_header.name, std::ios_base::in | std::ios_base::binary);

    if (!file.is_open()) {
        printf("Sorry but file [%s] could not be opened\n", file_header.name);
        return -1;
    }

    {
        struct FileContent content;
        int current_size = 0;
        while (!file.eof()) {
            memset(&content, 0, sizeof(content));
            file.read(content.buffer, FILE_BUFFER_SIZE);
            content.count = file.gcount();
            
            CHECK_IO((send(client_socket, (char*)&content, sizeof(content), 0) > 0), -1, "Can't send file content");
            CHECK_IO((recv(client_socket, (char*)&result, sizeof(result), 0) > 0), -1, "Can't send get sever response");
            CHECK_IO((result.status == FileTransferResult::OK), -1, "Server error to receive file");
    
            current_size += content.count;
            printf("\b\b\b\b\b%d %%", (int)(current_size * 1.0 / file_header.size * 100));
        }
    }

	return 0;
}

void exit_handler()
{
	if (client_socket > 0) {
		close_socket(client_socket);
	}
	
}
