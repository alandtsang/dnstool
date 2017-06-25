#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "packet.h"

const int dnshdr_off = 12;

uint16_t ip_sum(const uint16_t *hdr, int hdr_len)
{
    uint32_t sum = 0;

    while (hdr_len > 1) {
        sum += *hdr++;
        if (sum & 0x80000000)
            sum = (sum & 0xFFFF) + (sum >> 16);
        hdr_len -= 2;
    }

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ~sum;
}


void Packet::Init() {
	iphdr_ = (struct ipv4_hdr*) buf;
	udp = (struct udp_hdr*) (buf + sizeof(*iphdr_));
	dns = (struct dnshdr*) (buf + sizeof(*iphdr_) + sizeof(*udp));

	iphdr_->version_ihl     = IP_VHL_DEF;
	iphdr_->type_of_service=0;
	iphdr_->packet_id=0;
	iphdr_->fragment_offset = RTE_CPU_TO_BE_16(0x4000);
	iphdr_->protocol=17;
	iphdr_->hdr_checksum=0;

	udp->dgram_cksum = 0;

	dns->flags = RTE_CPU_TO_BE_16(0x0100);
	dns->qdcount = RTE_CPU_TO_BE_16(1);
	dns->ancount = 0;
	dns->nscount = 0;
	dns->arcount = 0;
}

void Packet::Set(const char* dstaddr, uint16_t port) {
	inet_aton(dstaddr, (struct in_addr *)&iphdr_->dst_addr);
	udp->dst_port = RTE_CPU_TO_BE_16(port);
}

void Packet::code_domain(std::string& domain)
{
    int start(0); // indexes
    std::size_t end;
	char *buffer = buf + 40;
	char *buffer2 = buf + 40;

    while ((end = domain.find('.', start)) != std::string::npos) {
        *buffer++ = end - start; // label length octet
        for (uint16_t i=start; i<end; i++) {
            *buffer++ = domain[i]; // label octets
        }
        start = end + 1; // Skip '.'
    }

    *buffer++ = domain.size() - start; // last label length octet
    for (uint16_t i=start; i<domain.size(); i++) {
        *buffer++ = domain[i]; // last label octets
    }

    *buffer++ = 0;
	query_len = buffer - buffer2 + 4;  // query name + type + class
}

void Packet::Generate(std::string& domain) {
	dns->id = RTE_CPU_TO_BE_16(rand() % 65535);

	/* query section */
	code_domain(domain);

	/* type and class */
	buf[40 + query_len - 4] = 0x00;
	buf[40 + query_len - 3] = 0x01;
	buf[40 + query_len - 2] = 0x00;
	buf[40 + query_len - 1] = 0x01;

	udp->dgram_len = RTE_CPU_TO_BE_16(8 + dnshdr_off + query_len);
	udp->src_port = RTE_CPU_TO_BE_16(rand()%65535+1);

	buf_len = 40 + query_len;
	iphdr_->total_length = RTE_CPU_TO_BE_16(buf_len);
	iphdr_->src_addr = htonl((rand()%3758096383));
	//iphdr_->src_addr=htonl(3232246921);
	iphdr_->time_to_live=IP_DEFTTL;

	return;
}

