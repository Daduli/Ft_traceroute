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

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

#define MAX_TTL 30

typedef struct s_host
{
    char *hostname;
    char ip_address[INET_ADDRSTRLEN];
} t_host;

typedef struct s_send_packet
{
    int sockfd;
    struct addrinfo *address_infos;
} t_send_packet;

typedef struct s_receive_packet
{
    int sockfd;
    struct addrinfo *address_infos;
} t_receive_packet;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_sending_socket(t_send_packet *send_packet, char *hostname);
void ft_receiving_socket(t_receive_packet *receive_packet);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
