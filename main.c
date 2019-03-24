// Wiktor Garbarek 291963

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "sender.h"
#include "receiver.h"
#include "printer.h"

#define UNDEF -1
#define TIMEOUT 1
#define REACHED 2
#define RECEIVED 3
#define N 3

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

void set_timeout(struct timeval *tv){
    tv->tv_sec = 1; tv->tv_usec = 0;
}

void traceroute(int sockfd, char *dest_ip){
    for(int ttl=1; ttl <= 30; ttl++){
        for(int i = 0; i < N; i++){
            send_icmp(sockfd, dest_ip, ttl);
        }
        struct timeval start_time, timedelta, end_time;
        struct timeval waiting_times[N];
        gettimeofday(&start_time, NULL);
        set_timeout(&timedelta);
        timeradd(&start_time, &timedelta, &end_time);

        char ips[N][20];
        printf("%d. ", ttl);
        int received_responses[N] = {-1, -1, -1};
        for(int i = 0; i < N; i++){
            uint16_t received_id = -1, received_ttl = -1;
            while((received_id != (uint16_t) getpid() || received_ttl != ttl) && received_responses[i] != TIMEOUT){
                received_responses[i] = receive_icmp(sockfd, &timedelta, &received_id, &received_ttl, ips[i]);
            }
            if (received_responses[i] == TIMEOUT){
                break;
            }
            gettimeofday(&waiting_times[i], NULL);
            timersub(&waiting_times[i], &start_time, &waiting_times[i]);
            if (timercmp(&timedelta, &waiting_times[i], <=)){
                break;
            }
            timersub(&timedelta, &waiting_times[i], &timedelta);

        }
        print_response(received_responses, ips, waiting_times);

        if(is_host_reached(received_responses)){
            fprintf(stderr, "reached\n");
            break;
        }
    }
}

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Invalid parameters or missing ip address\n");
		exit(EXIT_FAILURE);
	}

	struct in_addr addr;

	if (inet_aton(argv[1], &addr) == 0) {
        	fprintf(stderr, "Invalid address - expected form: x.x.x.x \n");
        	exit(EXIT_FAILURE);
    }
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    traceroute(sockfd, argv[1]);

    fprintf(stderr,"*|* %d *|*\n", getpid());
    close(sockfd);

}
