#include "nslookup.h"

int main(int argc, char* argv[])
{
	atexit(common_exit_handler);

	char domain_name[256];
	memset(domain_name, 0, sizeof domain_name);

	//Check command line argument
	//For example, using this program as 'nslookup www.domain_name.com'
	if (argc >= 2) {
		strcpy(domain_name, argv[1]);
	}
	else {
		printf("Enter domain name: ");
		scanf("%s", domain_name);
	}

	//Init WinSock2
	common_init_handler();
	//Retrieve DNS records
	hostent* he = gethostbyname(domain_name);
	if (he == NULL) {
		printf("%s\n", "Error DNS record retrieve");
		return -1;
	}
	//Loop ip list
	for (char** pa = he->h_addr_list; *pa != NULL; ++pa) {
		handle_he_ip(*pa, he->h_length);
	}

	return 0;
}