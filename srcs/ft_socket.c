#include "../ft_traceroute.h"

/*
 * Create a datagram socket for sending UDP packets
 */
void ft_create_send_socket(t_send_packet *send_packet, char *hostname)
{
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM; // Datagram socket to send an UDP packet

    if (getaddrinfo(hostname, PORT, &hints, &result) != 0)
    {
        printf("ft_traceroute: unknown host %s\n", hostname);
        exit(1);
    }

    send_packet->addr_in = *result->ai_addr;
    send_packet->addr_in_len = result->ai_addrlen;

    send_packet->sockfd = socket(PF_INET, result->ai_socktype, result->ai_protocol);
    if (send_packet->sockfd < 0)
    {
        printf("ft_traceroute: send socket creation failed\n");
        exit(1);
    }

    freeaddrinfo(result);
}

/*
 * Create a raw socket for receiving ICMP packets
 */
void ft_create_receive_socket(struct pollfd *receive_packet)
{
    receive_packet->fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (receive_packet->fd < 0)
    {
        printf("ft_traceroute: receive socket creation failed\n");
        exit(1);
    }
    receive_packet->events = POLLIN;
}