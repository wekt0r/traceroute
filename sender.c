#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

u_int16_t compute_icmp_checksum (const void *buff, int length){
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void fill_icmp_header(struct icmphdr *icmp_header, int ttl){
    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0;
    icmp_header->un.echo.id = (uint16_t) getpid(); //unikatowy identyfikator, np. PID
    icmp_header->un.echo.sequence = (uint16_t) ttl;    // numer sekwencyjny
    icmp_header->checksum = 0;
    icmp_header->checksum = compute_icmp_checksum ((u_int16_t*) icmp_header, sizeof(*icmp_header));
}

void send_icmp(int sockfd, const char *addr, int ttl){
    struct icmphdr icmp_header;
    fill_icmp_header(&icmp_header, ttl);

    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, addr, &recipient.sin_addr);
    if (inet_pton(AF_INET, addr, &recipient.sin_addr) != 1){
        handle_error("inet_pton");
    }
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) == -1){
        handle_error("setsockopt");
    }
    if(sendto(sockfd, &icmp_header, sizeof(icmp_header), 0,
              (struct sockaddr*) &recipient, sizeof(recipient)) < 0){
                  handle_error("sendto");
    }
}
