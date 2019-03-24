#ifndef PRINTER_H
#define PRINTER_H

#include <sys/time.h>

int is_host_reached(int *received_responses);
void print_response(int *rr, char ips[][20], struct timeval *waiting_times);

#endif
