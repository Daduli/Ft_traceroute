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
        probe_to_send->port++;
    }
}

/*
 * Sends a packet with a specified TTL and port value to the target host
 */
void ft_send_probe(t_send_packet *send_packet, t_probe *probe, t_cursor *probe_to_send)
{
    // Change this to a 60 bytes message
    char *message = "message";
    int port = atoi(PORT) + probe_to_send->port;
    // printf("Sending packet with port: %d\n", port);

    if (setsockopt(send_packet->sockfd, IPPROTO_IP, IP_TTL, &probe_to_send->ttl, sizeof(probe_to_send->ttl)) == -1)
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
    probe->ttl = probe_to_send->ttl;
    probe->probe_nb = probe_to_send->probe_nb;
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
            ft_send_probe(send_packet, &probes[i], probe_to_send);
            ft_advance_cursor(probe_to_send);
        }
    }
}

/*
 * Parse the packet outer IP and ICMP header then the inner IP and ICMP header if the ICMP type is TIME_EXCEEDED or ICMP_UNREACH
 * And  set the argument *port* from the port in the UDP header
 *
 * Returns 1 on success and 0 if the packet doesn't correspond to the mentinoned types
 */
int ft_parse_packet(char *buffer, uint16_t *port)
{
    struct ip *outer_ip = (struct ip *)buffer;
    int outer_ip_len = outer_ip->ip_hl * 4;

    struct icmp *outer_icmp = (struct icmp *)(buffer + outer_ip_len);

    if (outer_icmp->icmp_type == ICMP_TIME_EXCEEDED || outer_icmp->icmp_type == ICMP_UNREACH)
    {
        struct ip *orig_ip = (struct ip *)(buffer + outer_ip_len + 8); // 8 for ICMP header size (might change with #define)
        int orig_ip_len = orig_ip->ip_hl * 4;
        struct udphdr *orig_udp = (struct udphdr *)((char *)orig_ip + orig_ip_len); // UDP header follows IP header

        *port = ntohs(orig_udp->uh_dport);
        return (1);
    }
    return (0);
}

/*
 * Scan through the probes table to find if there is one that matches the port given
 *
 * Returns the index of the probes that matched, else return -1
 */
int ft_find_probe(t_probe *probes, uint16_t port)
{
    int i = -1;

    while (++i < QUERIES)
        if (probes[i].in_use && probes[i].port == port)
            return (i);
    return (-1);
}

void ft_receive_packet(struct pollfd *receive_packet, t_probe *probes)
{
    char buffer[1024];
    char src_ip[INET_ADDRSTRLEN];
    struct sockaddr *addr;
    socklen_t *addr_len;
    uint16_t port;
    int probe_index;

    recvfrom(receive_packet->fd, buffer, sizeof(buffer), 0, addr, addr_len);

    // Parse the packet, get outer IP and ICMP header then inner IP and ICMP header
    // Check if it's the correct ICMP type  and code (UNREACH || TIME_EXC)
    // Retrieve the original port that the packet was sent on
    if (!ft_parse_packet(buffer, &port))
        return;

    // printf("Port from packet received: %d\n", port);

    // Find in the probe table the one that have the same port
    probe_index = ft_find_probe(probes, port);
    // If not found, means that it's a packet we've already treated - return
    if (probe_index == -1)
        return;

    // Compute RTT, and save it in struct to print later

    // Stops if ICMP type = UNREACH???

    // Set the probe.in_use to false
    probes[probe_index].in_use = false;
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