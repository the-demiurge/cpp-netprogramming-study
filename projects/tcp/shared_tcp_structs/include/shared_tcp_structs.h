#ifndef _NETWORK_PROGRAMMING_SHARED_TCP_STRUCTS

#define _NETWORK_PROGRAMMING_SHARED_TCP_STRUCTS

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
#endif // !_NETWORK_PROGRAMMING_SHARED_TCP_STRUCTS
