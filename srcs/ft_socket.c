#include "../ft_traceroute.h"

/*
 * Create a datagram socket for sending UDP packets
 */
void ft_sending_socket(t_send_packet *send_packet, char *hostname)
{
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_DGRAM,
                        .ai_protocol = IPPROTO_UDP},
                    *result;

    if (getaddrinfo(hostname, NULL, &hints, &result) != 0)
    {
        printf("ft_traceroute: unknown host %s\n", hostname);
        exit(1);
    }
    send_packet->address = *(struct sockaddr_in *)result->ai_addr;
    send_packet->address.sin_port = htons(33434); // Default port for traceroute
    send_packet->sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (send_packet->sockfd < 0)
    {
        printf("ft_traceroute: socket creation failed\n");
        exit(1);
    }
    freeaddrinfo(result);
}

/*
 * Create a raw socket for receiving ICMP packets
 */
void ft_receiving_socket(t_receive_packet *receive_packet)
{
    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_RAW,
        .ai_protocol = IPPROTO_ICMP};

    if (getaddrinfo("localhost", NULL, &hints, &receive_packet->address_infos) != 0)
    {
        printf("ft_traceroute: unknown host localhost\n");
        exit(1);
    }

    receive_packet->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (receive_packet->sockfd < 0)
    {
        printf("ft_traceroute: socket creation failed\n");
        exit(1);
    }
}