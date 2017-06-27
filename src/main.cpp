#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
//#include <atomic>
//#include <chrono>
//#include <thread>
//#include <vector>

#include "sock.h"
#include "packet.h"
#include "data.h"


using namespace std;

struct timeval start_time, end_time;
static uint32_t delta;
static float time_use;
static float rate;
static uint64_t howmany;
static int counter;

struct server {
public:
	server() {
		ip = NULL;
		port = 53;
	}

	char* ip;
	uint16_t port;
};

static void signal_handler(int signum)
{
    if (signum == SIGKILL || signum == SIGINT) {
        gettimeofday(&end_time, NULL);
        delta = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
            (end_time.tv_usec - start_time.tv_usec);
        time_use = delta / 1000000.0;
        rate = howmany / time_use;

        std::cout << "Total = " << howmany << std::endl;
        std::cout << "Cost  = " << time_use << " seconds" << std::endl;
        std::cout << "Rate  = " << rate << " pps" << std::endl;

        exit(1);
    }
}

/* Display usage instructions */
static void print_usage(const char *prgname)
{
    printf("Usage: %s -s ip [-p port] [-n counts] -l local_ip -d datafile\n"
           "    -s ip: the server to query\n"
           "    -p port: the port on which to query the server\n"
           "    -l ip: the local address from which to send queries\n"
		   "    -n counts: repeatedly the datafile counts times\n"
           "    -d datafile: the input data file\n",
                prgname);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signal_handler);
    signal(SIGKILL, signal_handler);

	//using namespace std::chrono;
    //using clock=steady_clock;
	Socket socket;
	Packet pkt;
	Data data;

	std::string domain;
    const char* prgname = argv[0];

    int counts = 0;
	server s;
	int c;

	while ((c = getopt(argc, argv, "s:p:d:l:n:h")) != -1) {
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
            case 'l':
                pkt.sip = inet_addr(optarg);
                break;
            case 'n':
                counts = atoi(optarg);
                break;
            case 'h':
			default:
				print_usage(prgname);
                exit(EXIT_FAILURE);
		}
	}

	std::cout << "ip=" << s.ip << ", port=" << s.port << "\n";

	socket.Set(s.ip, s.port);
	pkt.Init();
	pkt.Set(s.ip, s.port);

	data.Open();

    char* p = NULL;
    char* ss = data.buffer;
    char* sss = data.buffer;
    int len = 0;

	gettimeofday(&start_time, NULL);
	if (counts) {
        while (1) {
            if (counter >= counts) break;

            while (1) {
                p = strchr(ss, '\n');
                if (p != NULL) {
                    len = p - ss;
                    domain.assign(ss, len); 
//std::cout << domain << "\n";
                    pkt.Generate(domain);
                    socket.Send(pkt.buf, pkt.buf_len);
                    ss = p + 1;
                    howmany++;
                } else {
                    break;
                }
            }

            counter++;
            ss = sss;
        }
    } else {
        while (1) {
            while (1) {
                p = strchr(ss, '\n');
                if (p != NULL) {
                    len = p - ss;
                    domain.assign(ss, len); 

                    pkt.Generate(domain);
                    socket.Send(pkt.buf, pkt.buf_len);
                    ss = p + 1;
                    howmany++;
                }
                if (*ss == '\0')
                    ss = sss;
            }
        }
	}

	//duration<float> delta = clock::now() - start;
    //float deltaf = delta.count();
    //auto rate = howmany/deltaf;
    gettimeofday(&end_time, NULL);
    delta = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
            (end_time.tv_usec - start_time.tv_usec);
    time_use = delta / 1000000.0;
    rate = howmany / time_use;

    std::cout << "Total = " << howmany << std::endl;
    std::cout << "Cost  = " << time_use << " seconds" << std::endl;
    std::cout << "Rate  = " << rate << " pps" << std::endl;

    return 0;
}
