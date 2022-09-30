#include "mouse_remote_manager.h"
void exit_handler();

SOCKET sender_socket;

int main(int argc, char* argv[])
{
    COMMAND_OPTIONS cmd_opts{DEFAULT_REMOTE_CONTROL_HOST, DEFAULT_REMOTE_CONTROL_PORT};
    parse_cmd(argc, argv, &cmd_opts);
    if (!valid_connection_opts(&cmd_opts))
    {
        common_usage(argv[0]);
        return -1;
    }

    atexit(common_exit_handler);
    atexit(exit_handler);
    common_init_handler();

    CHECK_IO((sender_socket = create_udp_socket()) > 0, -1, "Can't create socket\n");

    struct sockaddr_in receiver_address;
    init_inet_address(&receiver_address, cmd_opts.host, cmd_opts.port);
    socklen_t len = sizeof(receiver_address);

    printf("You connected to [%s:%d]. Move mouse and control it!",  cmd_opts.host, cmd_opts.port);

    char ch = 0;

    MOUSE_POSITION prev_mouse_pos = {0, 0};

    while(ch != 27) {
        ch = getc(stdin);

        MOUSE_POSITION current_mouse_pos = {0, 0};

        get_mouse_pos(&current_mouse_pos);

        if (is_mouse_moved(&prev_mouse_pos, &current_mouse_pos)) {
            int sc = sendto(sender_socket, (char*)&current_mouse_pos, sizeof(current_mouse_pos), 0, (sockaddr*)&receiver_address, len);

            if (sc <= 0) {
                char err_msg[512] = "";
                sprintf(err_msg, "Can't send data to the %s:%d", cmd_opts.host, cmd_opts.port);
                error_msg(err_msg);
            }

            prev_mouse_pos = current_mouse_pos;
        }

	}

	return 0;
}

bool is_mouse_moved(PMOUSE_POSITION prev, PMOUSE_POSITION current)
{
    return current->x != prev->x || current->y != prev->y;
}

void exit_handler()
{
	close_socket(sender_socket);
}
