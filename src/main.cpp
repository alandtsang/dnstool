#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
//#include <atomic>
//#include <chrono>
//#include <thread>
//#include <vector>

#include "sock.h"
#include "packet.h"


using namespace std;

static int counter = 0;

int main(int argc, char *argv[])
{
	//using namespace std::chrono;
    //using clock=steady_clock;
	int howmany;

	if (argc != 4) {
		printf("./dnstool ip port queries\n");
		exit(0);
	}
#if 0
	struct sockaddr_in dstaddress;
	dstaddress.sin_family=AF_INET;
	dstaddress.sin_port=RTE_CPU_TO_BE_16(atoi(argv[2]));
	dstaddress.sin_addr.s_addr = inet_addr(argv[1]);
#endif
	howmany = atoi(argv[3]);
#if 0
	const int on=1;
	int sk = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	setsockopt(sk,IPPROTO_IP,IP_HDRINCL,&on,sizeof(on));
#endif

	Socket socket;
	Packet pkt;

	socket.Set(argv[1], argv[2]);
	pkt.Init();
	pkt.Set(argv[1], argv[2]);

	uint32_t start = time(NULL);

	while (1) {
		if (counter++ >= howmany) break;

		pkt.Generate();
		socket.Send(pkt.buf, pkt.buf_len);
#if 0
		sendto(sk, pkt.buf, pkt.buf_len, 0, (struct sockaddr*) &dstaddress, 
				sizeof(struct sockaddr_in));
#endif
	}

	//duration<float> delta = clock::now() - start;
    //float deltaf = delta.count();
    //auto rate = howmany/deltaf;
    float delta = time(NULL) - start;
	float rate = howmany/delta;

    std::cout << "Total: " << howmany << std::endl;
    //std::cout << "Delta = " << deltaf << " seconds" << std::endl;
    std::cout << "Delta = " << delta << " seconds" << std::endl;
    std::cout << "Rate = " << rate << " pps" << std::endl;

    return 0;
}
