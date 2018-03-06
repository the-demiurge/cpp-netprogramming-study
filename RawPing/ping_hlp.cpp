#include "ping_hlp.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void parse_cmd_opts(const char* opt, PPING_CMD_OPTIONS opts)
{
	if (!strcmp(opt, "-r"))
	{
		opts->record_route = 1;
	}
	else
	{
		const char* p = strchr(opt, ':');
		if (p && strlen(p + 1))
		{
			if (!strncmp(opt, "-sz", 3))
			{
				opts->packet_size = atoi(p + 1);
			}
			else if (!strncmp(opt, "-t", 2))
			{
				opts->timeout = atoi(p + 1);
			}
			else if (!strncmp(opt, "-cn", 3))
			{
				opts->ping_count = atoi(p + 1);
			}
		}
	}
}

int parse_cmd(int argc, char* argv[], char* dest, PPING_CMD_OPTIONS opts)
{
	if (argc < 2) {
		return EPARSE_CMD_COUNT;
	}

	int i;
	for (i = 1; i < argc - 1; ++i) {
		parse_cmd_opts(argv[i], opts);
	}

	sprintf(dest, "%s", argv[argc - 1]);

	return EPARSE_CMD_OK;
}