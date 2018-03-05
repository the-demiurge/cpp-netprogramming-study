#include "ping_hlp.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int parse_cmd_opts(const char* sopts, PPING_CMD_OPTIONS opts)
{
	if (!sopts || !strlen(sopts))
	{
		return EPARSE_CMD_OK;
	}

	char rec_rt_ch = '\0';

	char format[64] = "-sz:%d -%c -t:%d";

	if (sscanf(sopts, format, &(opts->packet_size),
		&rec_rt_ch, &(opts->timeout)) > 0)
	{
		opts->record_route = tolower(rec_rt_ch) == 'r';
		return EPARSE_CMD_OK;
	}

	return EPARSE_CMD_ANY;
}

int parse_cmd(int argc, char* argv[], char* dest, PPING_CMD_OPTIONS opts)
{
	if (argc < 2) {
		return EPARSE_CMD_COUNT;
	}

	char options[256] = "";
	int i;
	for (i = 1; i < argc - 1; ++i) {
		sprintf(options, "%s%c", i + 1 < argc - 1 ? ' ' : '\0');
	}

	if (parse_cmd_opts(options, opts))
	{
		return EPARSE_CMD_OPTIONS;
	}

	sprintf(dest, "%s", argv[argc - 1]);

	return EPARSE_CMD_OK;
}