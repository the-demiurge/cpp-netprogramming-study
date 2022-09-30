#ifndef _NETWORK_PROGRAMMING_COMMON_PROTOCOLS

#define _NETWORK_PROGRAMMING_COMMON_PROTOCOLS

struct FileTransferRequest {
	char file_size_str[32];
	char file_name[32];
	char* bytes = new char[atoi(file_size_str)];
};
enum FileTransferStatus { OK = 0, BIG_SIZE = 1, SAVED = 2 };

struct FileTransferResponse {
	enum FileTransferStatus status;
};

struct SquareRootRequest {
	double a;
	double b;
	double c;
};

enum RootStatus { NO_ROOTS = 0, ONE_ROOTS = 1, TWO_ROOTS = 2 };

struct SquareRootResponse {
	double x1;
	double x2;
	enum RootStatus status;
};


struct MessagePacket {
    char data[256];
};

struct WordPacket {
    char data[256];
    int words;
};
#endif // !_NETWORK_PROGRAMMING_COMMON_PROTOCOLS
