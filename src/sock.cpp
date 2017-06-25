#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sock.h"


void Socket::Set(const char* ip, uint16_t port) {
	addr_.sin_family = AF_INET;
    addr_.sin_port = RTE_CPU_TO_BE_16(port);
    addr_.sin_addr.s_addr = inet_addr(ip);

	sockfd_ = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	const int on = 1;
	setsockopt(sockfd_, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));
}

void Socket::Send(const void* buf, uint16_t len) {
	sendto(sockfd_, buf, len, 0, (struct sockaddr*) &addr_, 
			sizeof(struct sockaddr_in));
}

