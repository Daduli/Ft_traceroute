#include "../ft_traceroute.h"

void ft_send_packet(t_send_packet *send_packet, int ttl)
{
    char *message = "message";

    if (setsockopt(send_packet->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
    {
        printf("ft_traceroute: setsockopt failed\n");
        close(send_packet->sockfd);
        exit(1);
    }

    if (sendto(send_packet->sockfd, message, sizeof(message), 0, (struct sockaddr *)&send_packet->address, sizeof(send_packet->address)) == -1)
    {
        printf("ft_traceroute: sendto failed: %s\n", strerror(errno));
        close(send_packet->sockfd);
        exit(1);
    }
}

// void ft_receive_packet(int sockfd)
// {
// }