#include "../ft_traceroute.h"

/*
 * Sends a packet with a specified TTL value to the target host
 */
void ft_send_packet(t_send_packet *send_packet, int ttl)
{
    // Change this to a 60 bytes message
    char *message = "message";

    if (setsockopt(send_packet->sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
    {
        printf("ft_traceroute: send setsockopt failed\n");
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

/*
 * Receives an ICMP packet and returns a code indicating the type of response received:
 *
 * Code -1 = Receive Timeout
 * Code 0 = ICMP Time Exceeded
 * Code 1 = ICMP Port Unreachable
 */
void ft_receive_packet(t_receive_packet *receive_packet)
{
    char buffer[1024];
    struct timeval timeout;
    timeout.tv_sec = 5; // Set timeout to 5 seconds

    if (setsockopt(receive_packet->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        printf("ft_traceroute: receive setsockopt failed\n");
        close(receive_packet->sockfd);
        exit(1);
    }

    if (recvfrom(receive_packet->sockfd, buffer, sizeof(buffer), 0, receive_packet->address_infos->ai_addr, &receive_packet->address_infos->ai_addrlen) == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("*\n");
        }
        else
        {
            printf("ft_traceroute: recvfrom failed: %s\n", strerror(errno));
            close(receive_packet->sockfd);
            exit(1);
        }
    }

    struct iphdr *ip_header = (struct iphdr *)buffer;
    struct icmphdr *icmp_header = (struct icmphdr *)(buffer + ip_header->ihl * 4);

    if (icmp_header->type == ICMP_TIME_EXCEEDED && icmp_header->code == ICMP_EXC_TTL)
        printf("TTL Exceeded\n");
    else if (icmp_header->type == ICMP_DEST_UNREACH && icmp_header->code == ICMP_PORT_UNREACH)
        printf("Host Reached\n");
}