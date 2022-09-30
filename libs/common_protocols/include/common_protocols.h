#ifndef _NETWORK_PROGRAMMING_COMMON_PROTOCOLS

#define _NETWORK_PROGRAMMING_COMMON_PROTOCOLS

const int FILE_HEADER_SIZE = 32;

const int FILE_BUFFER_SIZE = 1024;

struct FileHeader {
    char name[FILE_HEADER_SIZE];
	unsigned long size;
};

struct FileContent {
    char buffer[FILE_BUFFER_SIZE];
    long count;
	bool is_closed;
};

struct FileTransferResult {
    enum FileAcceptStatus { ACCEPTED = 0, NOT_ACCEPTED_SIZE = 1, OK = 2, FAIL = 3 };
	enum FileAcceptStatus status;
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
