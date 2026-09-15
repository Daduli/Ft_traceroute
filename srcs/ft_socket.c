#include "../ft_traceroute.h"

/*
 * Create a datagram socket for sending UDP packets
 */
void ft_sending_socket(t_packet *send_packet, char *hostname)
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

    send_packet->address_infos = *result->ai_addr;
    send_packet->address_infos_len = result->ai_addrlen;

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
void ft_receiving_socket(t_probe *probe)
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct pollfd poll_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP; // Receive ICMP packets

    if (getaddrinfo("localhost", NULL, &hints, &result) != 0)
    {
        printf("ft_traceroute: unknown host localhost\n");
        exit(1);
    }

    poll_fd.fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (poll_fd.fd < 0)
    {
        printf("ft_traceroute: receive socket creation failed\n");
        exit(1);
    }
    poll_fd.events = POLLIN;

    probe->poll_fd = poll_fd;

    freeaddrinfo(result);
}