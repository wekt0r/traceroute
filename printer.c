// Wiktor Garbarek 291963

#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "constants.h"

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

double avg_timeval(int n, struct timeval *times){
	double res = 0.0;
	for (int i = 0; i < n; i++){
		res += times[i].tv_usec/(n*1000.0);
	}
	return res;
}

void print_response(int *rr, char ips[][20], struct timeval *waiting_times){
    int responses_count = (rr[0] > TIMEOUT) + (rr[1] > TIMEOUT) + (rr[2] > TIMEOUT);
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
	    for (int i = 2; i >= 0; i--){
	    	if (ips[i][0] != 0){ 
		    printf("%s ", ips[i]);
		}
	    }
            printf("%.0fms\n", avg_timeval(3, waiting_times));
            break;
    }

}
