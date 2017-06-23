#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "packet.h"


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

void Packet::Set(const char* dstaddr, const char* port) {
	inet_aton(dstaddr, (struct in_addr *)&iphdr_->dst_addr);
	udp->dst_port = RTE_CPU_TO_BE_16(atoi(port));
}

void Packet::Generate() {
	dns->id = RTE_CPU_TO_BE_16(rand() % 65535);
	int namelen=rand()%6+2;
	char p;
	p=(char)namelen;

	memcpy(buf+40,&p,1);
	int i=0;
	char *pi=buf+41;
	for(i=0;i<namelen;i++) {
		int a=rand()%26+97;
		char ca=(char) a;
		memcpy(pi,&ca,1);
		pi++;
	}

	const char *domain[]={"com","io","cn"};
	int dii=rand()%3;
	int domlen=strlen(domain[dii]);
	char cdomlen=(char)domlen;
	memcpy(buf+41+namelen,&cdomlen,1);
	memcpy(buf+42+namelen,domain[dii],domlen);
	int zero=0;
	char czero=(char)zero;
	memcpy(buf+42+namelen+domlen,&czero,1);
	int type_class=htonl(0x00010001);
	memcpy(buf+43+namelen+domlen,&type_class,4);

	udp->dgram_len = RTE_CPU_TO_BE_16(27+namelen+domlen);
	udp->src_port = RTE_CPU_TO_BE_16(rand()%65535+1);

	buf_len = 47+namelen+domlen;
	iphdr_->total_length = RTE_CPU_TO_BE_16(buf_len);
	iphdr_->src_addr = htonl((rand()%3758096383));
	//iphdr_->src_addr=htonl(3232246921);
	iphdr_->time_to_live=IP_DEFTTL;
	//iphdr_->hdr_checksum = ip_sum((uint16_t *)iphdr_, sizeof(*iphdr_));

	return;
}

