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
#include <time.h>
#include <stdbool.h>

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

#define MAX_TTL 30
#define NB_PROBES 3
#define START_TTL 1
#define QUERIES 16
#define PORT "33434"

typedef struct s_send_packet
{
    int sockfd;
    struct sockaddr addr_in;
    size_t addr_in_len;
} t_send_packet;

typedef struct s_probe
{
    int ttl;
    int port;
    int query_nb;
    bool in_use;
    struct timeval send_time;
} t_probe;

typedef struct s_cursor
{
    int next_query;
    int next_ttl;
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

// void ft_send_packet(t_packet *send_packet, t_probe *probe, int ttl);
// void ft_receive_packet(t_packet *receive_packet, t_probe *probes, struct pollfd *poll_fd);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
