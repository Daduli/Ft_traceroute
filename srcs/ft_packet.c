#include "../ft_traceroute.h"

// void ft_send_packet(int ttl, int sockfd, t_packet_info *packet_info)
// {
//     char message[60];

//     if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
//     {
//         printf("ft_traceroute: setsockopt failed\n");
//         close(sockfd);
//         exit(1);
//     }

//     if (sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&packet_info->socket_address, sizeof(packet_info->socket_address)) == -1)
//     {
//         printf("ft_traceroute: sendto failed\n");
//         close(sockfd);
//         exit(1);
//     }
// }

// void ft_receive_packet(int sockfd)
// {
// }