#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#define UNDEF -1
#define TIMEOUT 1
#define REACHED 2
#define RECEIVED 3
#define N 3

int is_host_reached(int *received_responses){
    return received_responses[0] == REACHED || received_responses[1] == REACHED || received_responses[2] == REACHED;
}

void remove_duplicates(char ips[][20]){
    qsort(ips, 3, 20, (int (*)(const void *, const void *))strcmp);
    if (strcmp(ips[1], ips[2]) == 0){
        bzero(ips[2], 20);
    }
    if (strcmp(ips[0], ips[1]) == 0){
        bzero(ips[1], 20);
    }
    qsort(ips, 3, 20, (int (*)(const void *, const void *))strcmp);
}

void print_response(int *rr, char ips[][20], struct timeval *waiting_times){
    int responses_count = (rr[0] > TIMEOUT) + (rr[1] > TIMEOUT) + (rr[2] > TIMEOUT);
    //fprintf(stderr, "-rr----> %d, %d, %d\n", rr[0], rr[1], rr[2]);
    switch (responses_count) {
        case 0:
            printf("* \n");
            break;
        case 1:
            printf("%s ???\n", ips[0]);
            break;
        case 2:
            if (strcmp(ips[0], ips[1]) == 0){
                printf("%s ???\n", ips[0]);
            } else {
                 printf("%s %s ???\n", ips[0], ips[1]);
            }
            break;
        case 3:
            remove_duplicates(ips);
            printf("%s %s %s %.0fms\n", ips[2], ips[1], ips[0],  (waiting_times[0].tv_usec/1000.0 + waiting_times[1].tv_usec/1000.0 + waiting_times[2].tv_usec/1000.0)/3.0 );
            break;
    }

}
