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

typedef struct s_icmp_packet
{
    struct icmphdr header;
    char data[60];
} t_icmp_packet;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
