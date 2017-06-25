#ifndef INCLUDE_SOCK_H_
#define INCLUDE_SOCK_H_

#include <cstdint>

#include <netinet/in.h>

#include "utils.h"


class Socket {
public:
	Socket() {}
	~Socket() {}

	void Set(const char* ip, uint16_t port);
	void Send(const void* buf, uint16_t len);

private:
	int sockfd_;
	struct sockaddr_in addr_;
};


#endif
