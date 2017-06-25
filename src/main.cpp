#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
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
#include "data.h"


using namespace std;


struct server {
public:
	server() {
		ip = NULL;
		port = 0;
	}

	char* ip;
	uint16_t port;
};

int main(int argc, char *argv[])
{
	//using namespace std::chrono;
    //using clock=steady_clock;
	Socket socket;
	Packet pkt;
	Data data;

	std::string domain;

	int howmany = 0;
	server s;
	int c;

	while ((c = getopt(argc, argv, "s:p:d:n:")) != -1) {
		switch (c) {
			case 's':
				s.ip = optarg;
				break;
			case 'p':
				s.port = atoi(optarg);
				break;
			case 'd':
				data.filename = optarg;
				break;
			case 'n':
				break;
			default:
				break;
		}
	}

	std::cout << "ip=" << s.ip << ", port=" << s.port << "\n";

	socket.Set(s.ip, s.port);
	pkt.Init();
	pkt.Set(s.ip, s.port);

	data.Open();

	uint32_t start = time(NULL);
	while (getline(data.ifs, domain)) {
		pkt.Generate(domain);
		socket.Send(pkt.buf, pkt.buf_len);
		howmany++;
	}

	//duration<float> delta = clock::now() - start;
    //float deltaf = delta.count();
    //auto rate = howmany/deltaf;
    float delta = time(NULL) - start;
	float rate = howmany/delta;

    std::cout << "Total: " << howmany << std::endl;
    std::cout << "Delta = " << delta << " seconds" << std::endl;
    std::cout << "Rate = " << rate << " pps" << std::endl;

    return 0;
}
