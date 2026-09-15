#pragma once

/*================================*/
/*==========    LIBC    ==========*/
/*================================*/

#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <poll.h>

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

#define MAX_TTL 30
#define NB_PROBES 3
#define START_TTL 1
#define QUERIES 16
#define PORT "33434"

typedef struct s_packet
{
    int sockfd;
    struct sockaddr address_infos;
    size_t address_infos_len;
} t_packet;

typedef struct s_probe
{
    struct pollfd poll_fd;
    struct timespec start_time;
    struct timespec end_time;
    int probe_nb;
    int port;
} t_probe;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_sending_socket(t_packet *send_packet, char *hostname);
void ft_receiving_socket(t_probe *probe);

/*================================*/
/*==========    PACKET    ========*/
/*================================*/

// void ft_send_packet(t_packet *send_packet, int ttl);
// void ft_receive_packet(t_packet *receive_packet);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
