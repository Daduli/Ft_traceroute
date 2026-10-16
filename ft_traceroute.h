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
#include <ctype.h>

/*================================*/
/*=====    DATA STRUCTURE    =====*/
/*================================*/

#define MAX_TTL 30 // Drop from bonus, keep as constant
#define NB_PROBES 3
#define START_TTL 1 // Drop from bonus
#define QUERIES 16
#define WAIT_TIME_MS 5000
#define PROBE_PER_HOP 3

#define HOSTNAME 'H'
#define SINGLE_DASH 'S'
#define DOUBLE_DASH 'D'

#define PORT 'p'
#define DNS_RESOLUTION 'n'
#define PROBE_COUNT 'N'
#define WAIT_TIME 'w'

typedef struct s_flags
{
    uint16_t port;
    bool dns_resolution;
    int probe_count;
    int max_wait;
} t_flags;

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
    int probe_nb; // Maybe remove this variable if not used for results
    bool in_use;
    struct timespec send_time;
} t_probe;

typedef struct s_cursor
{
    int ttl;
    int probe_nb;
    int port;
} t_cursor;

typedef struct s_probe_result
{
    bool replied;
    float rtt;
    struct sockaddr_in *addr_in;
} t_probe_result;

typedef struct s_hop_result
{
    int ttl;
    float rtt; // Earliest RTT
    int replies;
    bool printable;
    t_probe_result res[3];
} t_hop_result;

/*================================*/
/*==========    PARSE    =========*/
/*================================*/

void ft_parser(int ac, char **av, char **hostname, t_flags *flags);

/*================================*/
/*==========    SOCKET    ========*/
/*================================*/

void ft_create_send_socket(t_send_packet *send_packet, char *hostname, uint16_t port);
void ft_create_receive_socket(struct pollfd *receive_packet);

/*================================*/
/*==========    PACKET    ========*/
/*================================*/

void ft_send_packet(t_send_packet *send_packet, t_probe *probes, t_cursor *probe_to_send, t_flags flags);
void ft_receive_packet(struct pollfd *receive_packet, t_probe *probes, t_hop_result *results, int *next_print, int probe_count);
void ft_expired_packet(t_hop_result *results, t_probe *probes, int *next_print, int probe_count);

/*================================*/
/*=========    RESULTS    ========*/
/*================================*/

void ft_init_results(t_hop_result *results);
void ft_record_result(t_hop_result *results, t_probe *probe, struct sockaddr_in *addr_in, float rtt, bool replied, int *next_print);

/*================================*/
/*=========    HELPERS    ========*/
/*================================*/

void print_help();
t_probe *init_probe(t_cursor *probe_to_send, size_t probe_nb);
float compute_timeout(t_probe *probes, t_hop_result *results, t_flags flags);
