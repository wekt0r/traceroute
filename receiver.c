#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sys/select.h>

#include "receiver.h"

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct icmphdr* get_icmp_header(struct iphdr* ip_header){
    return (void*)ip_header + 4*ip_header->ihl;
}

struct iphdr* get_inner_ip_header(struct icmphdr* icmp_header){
    return (void*)icmp_header + 8;
}

int receive_icmp(int sockfd, struct timeval *timedelta, uint16_t *received_id, uint16_t *received_ttl, char *ip){
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];

    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);

    int ready = select(sockfd+1, &descriptors, NULL, NULL, timedelta);
    if (ready < 0){
        handle_error("select");
    }
    if (ready == 0){
        return TIMEOUT;
    }

    if (recvfrom(sockfd, buffer, IP_MAXPACKET, 0,
                 (struct sockaddr*)&sender, &sender_len) < 0){
        handle_error("recvfrom");
    }


    char sender_ip_str[20];
    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    sprintf(ip, "%s", sender_ip_str);

    struct iphdr* ip_header = (struct iphdr*) buffer;
    struct icmphdr* icmp_header = get_icmp_header(ip_header);

    *received_id = icmp_header->un.echo.id;
    *received_ttl = icmp_header->un.echo.sequence;
    if (icmp_header->type == ICMP_TIME_EXCEEDED){
        struct iphdr* ip_header2 = get_inner_ip_header(icmp_header);
        struct icmphdr* icmp_header2 = get_icmp_header(ip_header2);
        *received_id = icmp_header2->un.echo.id;
        *received_ttl = icmp_header2->un.echo.sequence;
    }

    if (icmp_header->type == ICMP_ECHOREPLY){
        return REACHED;
    }
    return RECEIVED;
}
