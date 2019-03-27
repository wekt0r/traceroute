#ifndef SENDER_H
#define SENDER_H

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

void send_icmp(int sockfd, const char *addr, int ttl, int i);

#endif
