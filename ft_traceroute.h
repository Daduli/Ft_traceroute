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
#include <netinet/udp.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <sys/socket.h>
#include <poll.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

#define MAX_TTL 30
#define NB_PROBES 3
#define START_TTL 1
#define QUERIES 16
#define PORT "33434"
#define WAIT_TIME_MS 5000

typedef unsigned long ul;

typedef struct s_send_packet
{
    int sockfd;
    struct sockaddr_in *addr_in;
    size_t addr_in_len;
} t_send_packet;

typedef struct s_probe
{
    int ttl;
    int port;
    int probe_nb;
    bool in_use;
    struct timespec send_time;
} t_probe;

typedef struct s_cursor
{
    int ttl;
    int probe_nb;
    int port;
} t_cursor;

typedef struct s_query_result
{
    bool replied;
    char ip[INET_ADDRSTRLEN];
    char *hostname;
    int rtt;
} t_query_result;

typedef struct s_hop_result
{
    t_query_result res[3];
    int replies;
} t_hop_result;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_create_send_socket(t_send_packet *send_packet, char *hostname);
void ft_create_receive_socket(struct pollfd *receive_packet);

/*================================*/
/*==========    PACKET    ========*/
/*================================*/

void ft_send_packet(t_send_packet *send_packet, t_probe *probes, t_cursor *probe_to_send);
void ft_receive_packet(struct pollfd *receive_packet, t_probe *probes);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
t_probe *init_probe(t_cursor *probe_to_send);
float compute_timeout(t_probe *probes);