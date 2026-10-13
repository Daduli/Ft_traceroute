#include "../ft_traceroute.h"

extern bool reached;
extern int final_ttl;

/*
 * Move the cursor to the next probe to be sent in the queries list
 */
void ft_advance_cursor(t_cursor *probe_to_send)
{
    probe_to_send->probe_nb++;
    if (probe_to_send->probe_nb == PROBE_PER_HOP)
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
        if (outer_icmp->icmp_type == ICMP_TIME_EXCEEDED)
            return (ICMP_TIME_EXCEEDED);
        return (ICMP_UNREACH);
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

void ft_receive_packet(struct pollfd *receive_packet, t_probe *probes, t_hop_result *results, int *next_print)
{
    char buffer[1024];
    char src_ip[INET_ADDRSTRLEN];
    char sender_hostname[256];
    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    uint16_t port;
    int probe_index;
    struct timespec end_time;
    int icmp_resp;

    recvfrom(receive_packet->fd, buffer, sizeof(buffer), 0, &addr, &addr_len);

    if (!(icmp_resp = ft_parse_packet(buffer, &port)))
        return;

    // Find in the probe table the one that have the same port
    probe_index = ft_find_probe(probes, port);
    // If not found, means that it's a packet we've already treated - return
    if (probe_index == -1)
        return;

    if (icmp_resp == ICMP_UNREACH)
    {
        reached = true;
        final_ttl = probes[probe_index].ttl;
    }

    // Compute RTT, and save it in struct to print later
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    end_time.tv_sec -= probes[probe_index].send_time.tv_sec;
    end_time.tv_nsec -= probes[probe_index].send_time.tv_nsec;
    float time_ms = (float)end_time.tv_sec * 1000 + (float)end_time.tv_nsec / 1000000;

    // Record results for printing
    ft_record_result(results, &probes[probe_index], (struct sockaddr_in *)&addr, time_ms, true, next_print);

    // Stops if ICMP type = UNREACH???

    // Set the probe.in_use to false
    probes[probe_index].in_use = false;
}

void ft_expired_packet(t_hop_result *results, t_probe *probes, int *next_print)
{
    struct timespec now;
    float deadline = INFINITY;

    clock_gettime(CLOCK_MONOTONIC, &now);
    float now_ms = now.tv_sec * 1000 + now.tv_nsec / 1000000;

    for (int i = 0; i < QUERIES; i++)
    {
        deadline = probes[i].send_time.tv_sec * 1000 + probes[i].send_time.tv_nsec / 1000000 + WAIT_TIME_MS;
        if (now_ms > deadline && probes[i].in_use)
        {
            // Record probe for result
            ft_record_result(results, &probes[i], NULL, 0.0f, false, next_print);
            probes[i].in_use = false;
        }
    }
}