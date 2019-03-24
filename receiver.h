#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>

#define UNDEF -1
#define TIMEOUT 1
#define REACHED 2
#define RECEIVED 3
#define N 3

int receive_icmp(int sockfd, struct timeval *timedelta, uint16_t *received_id, uint16_t *received_ttl, char *ip);

#endif
