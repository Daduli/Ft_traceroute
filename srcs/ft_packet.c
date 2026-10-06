#include "../ft_traceroute.h"

/*
 * Move the cursor to the next probe to be sent in the queries list
 */
void ft_advance_cursor(t_cursor *probe_to_send)
{
    probe_to_send->probe_nb++;
    if (probe_to_send->probe_nb == 3)
    {
        probe_to_send->probe_nb = 0;
        probe_to_send->ttl++;
    }
}

/*
 * Sends a packet with a specified TTL and port value to the target host
 */
void ft_send_probe(t_send_packet *send_packet, t_probe *probe, t_cursor probe_to_send)
{
    // Change this to a 60 bytes message
    char *message = "message";
    int port = atoi(PORT) + probe_to_send.port++;

    if (setsockopt(send_packet->sockfd, IPPROTO_IP, IP_TTL, &probe_to_send.ttl, sizeof(probe_to_send.ttl)) == -1)
    {
        printf("ft_traceroute: send setsockopt failed\n");
        close(send_packet->sockfd);
        exit(1);
    }
    send_packet->addr_in->sin_port = htons(port);

    if (sendto(send_packet->sockfd, message, sizeof(message), 0, (struct sockaddr *)send_packet->addr_in, send_packet->addr_in_len) == -1)
    {
        printf("ft_traceroute: sendto failed: %s\n", strerror(errno));
        close(send_packet->sockfd);
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &probe->send_time);
    probe->in_use = true;
    probe->ttl = probe_to_send.ttl;
    probe->probe_nb = probe_to_send.probe_nb;
    probe->port = port;
}

/*
 * Sends QUERIES probes simultaneously
 */
void ft_send_packet(t_send_packet *send_packet, t_probe *probes, t_cursor *probe_to_send)
{
    for (int i = 0; i < QUERIES; i++)
    {
        if (probe_to_send->ttl > MAX_TTL)
            return;
        if (!probes[i].in_use)
        {
            ft_send_probe(send_packet, &probes[i], *probe_to_send);
            ft_advance_cursor(probe_to_send);
        }
    }
}

void ft_receive_packet(struct pollfd *receive_packet, t_probe *probes)
{
    char buffer[1024];
    char src_ip[INET_ADDRSTRLEN];
    struct sockaddr *addr;
    socklen_t *addr_len;

    recvfrom(receive_packet->fd, buffer, sizeof(buffer), 0, addr, addr_len);
    printf("Packet received!\n");
}

// void ft_receive_packet(t_packet *receive_packet, t_probe *probes, struct pollfd *poll_fd)
// {
//     int num_events = poll(poll_fd, QUERIES, 5000);
//     char buffer[1024];

//     // printf("ft_traceroute: Number of events: %d\n", num_events);

//     if (num_events == 0)
//     {
//         printf("ft_traceroute: poll timeout\n");
//         return;
//     }
//     else
//     {
//         for (int i = 0; i < QUERIES; i++)
//         {
//             // printf();
//             if (poll_fd[i].revents & POLLIN)
//             {
//                 printf("Received response\n");
//                 recvfrom(poll_fd[i].fd, buffer, sizeof(buffer), 0, &receive_packet->address_infos, (socklen_t *)&receive_packet->address_infos_len);

//                 // Get the sender's IP address
//                 char sender_ip[INET_ADDRSTRLEN];
//                 inet_ntop(AF_INET, &receive_packet->address_infos, sender_ip, sizeof(sender_ip));

//                 struct sockaddr_in *addr_in = (struct sockaddr_in *)&receive_packet->address_infos;
//                 inet_ntop(AF_INET, &(addr_in->sin_addr), sender_ip, sizeof(sender_ip));
//                 printf("Sender IP: %s\n", sender_ip);

//                 // // Get the sender's hostname
//                 // char sender_hostname[256];
//                 // getnameinfo(receive_packet->address_infos->ai_addr, receive_packet->address_infos->ai_addrlen, sender_hostname, 256, NULL, 0, 0);
//                 // printf("Hostname : %s\n", sender_hostname);
//             }
//             else
//                 printf("No response\n");
//         }
//     }
// }

// /*
//  * Receives an ICMP packet and returns a code indicating the type of response received:
//  *
//  * Code -1 = Receive Timeout
//  * Code 0 = ICMP Time Exceeded
//  * Code 1 = ICMP Port Unreachable
//  */
// // void ft_receive_packet(t_receive_packet *receive_packet)
// // {
// //     char buffer[1024];
// //     struct timeval timeout;
// //     timeout.tv_sec = 1;  // Set timeout to 5 seconds
// //     timeout.tv_usec = 0; // It is mandatory to set usec

// //     if (setsockopt(receive_packet->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
// //     {
// //         printf("ft_traceroute: receive setsockopt failed, %s\n", strerror(errno));
// //         close(receive_packet->sockfd);
// //         exit(1);
// //     }

// //     if (recvfrom(receive_packet->sockfd, buffer, sizeof(buffer), 0, receive_packet->address_infos->ai_addr, &receive_packet->address_infos->ai_addrlen) == -1)
// //     {
// //         if (errno == EAGAIN || errno == EWOULDBLOCK)
// //         {
// //             printf("*\n");
// //         }
// //         else
// //         {
// //             printf("ft_traceroute: recvfrom failed: %s\n", strerror(errno));
// //             close(receive_packet->sockfd);
// //             exit(1);
// //         }
// //     }

// //     // Get the sender's IP address
// //     char sender_ip[INET_ADDRSTRLEN];
// //     inet_ntop(AF_INET, &((struct sockaddr_in *)receive_packet->address_infos->ai_addr)->sin_addr, sender_ip, sizeof(sender_ip));
// //     printf("Sender IP: %s\n", sender_ip);

// //     // Get the sender's hostname
// //     char sender_hostname[256];
// //     getnameinfo(receive_packet->address_infos->ai_addr, receive_packet->address_infos->ai_addrlen, sender_hostname, 256, NULL, 0, 0);
// //     printf("Hostname : %s\n", sender_hostname);

// //     // Set up the ICMP header and IP header pointers to detect the ICMP response type
// //     struct iphdr *ip_header = (struct iphdr *)buffer;
// //     struct icmphdr *icmp_header = (struct icmphdr *)(buffer + ip_header->ihl * 4);

// //     if (icmp_header->type == ICMP_TIME_EXCEEDED && icmp_header->code == ICMP_EXC_TTL)
// //         printf("TTL Exceeded\n");
// //     else if (icmp_header->type == ICMP_DEST_UNREACH && icmp_header->code == ICMP_PORT_UNREACH)
// //     {
// //         printf("Host Reached\n");
// //         exit(0);
// //     }
// // }