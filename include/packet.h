#ifndef INCLUDE_PACKET_H_
#define INCLUDE_PACKET_H_

#include <cstdint>
#include <string>

#include "utils.h"


#define IP_DEFTTL  64   /* from RFC 1340. */
#define IP_VERSION 0x40
#define IP_HDRLEN  0x05 /* default IP header length == five 32-bits words. */
#define IP_VHL_DEF (IP_VERSION | IP_HDRLEN)


struct ipv4_hdr {
	uint8_t  version_ihl;		/**< version and header length */
	uint8_t  type_of_service;	/**< type of service */
	uint16_t total_length;		/**< length of packet */
	uint16_t packet_id;		/**< packet ID */
	uint16_t fragment_offset;	/**< fragmentation offset */
	uint8_t  time_to_live;		/**< time to live */
	uint8_t  protocol;		/**< protocol ID */
	uint16_t hdr_checksum;		/**< header checksum */
	uint32_t src_addr;		/**< source address */
	uint32_t dst_addr;		/**< destination address */
};

struct udp_hdr {
	uint16_t src_port;    /**< UDP source port. */
	uint16_t dst_port;    /**< UDP destination port. */
	uint16_t dgram_len;   /**< UDP datagram length */
	uint16_t dgram_cksum; /**< UDP datagram checksum */
};

struct dnshdr {
	uint16_t id;		/* query identification number */
	uint16_t flags;		/* first byte of flags */
	uint16_t qdcount;	/* number of question entries */
	uint16_t ancount;	/* number of answer entries */
	uint16_t nscount;	/* number of authority entries */
	uint16_t arcount;	/* number of resource entries */
};

class Packet {
public:
	Packet() {}
	~Packet() {}

	void Init();
	void Set(const char* dstaddr, uint16_t port);
	void Generate(std::string& domain);

	char buf[256];
	uint16_t buf_len;
    uint32_t sip;

private:
	void code_domain(std::string& domain);

	struct ipv4_hdr*	iphdr_;
	struct udp_hdr*		udp;
	struct dnshdr*		dns;

	uint8_t query_len;
};



#endif
