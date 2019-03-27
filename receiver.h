#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

int receive_icmp(int sockfd, struct timeval *timedelta, uint16_t *received_id, uint16_t *received_ttl, char *ip);

#endif
