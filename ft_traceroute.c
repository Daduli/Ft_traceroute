#include "ft_traceroute.h"

volatile bool reached = false;
volatile int final_ttl = MAX_TTL;

int main(int ac, char **av)
{
    char *hostname;
    char dest_ip[INET_ADDRSTRLEN];
    t_send_packet send_packet;
    struct pollfd receive_packet[1];
    t_probe *probes;
    t_cursor probe_to_send;
    int num_events;
    float timeout;
    t_hop_result results[30];
    int next_print = 0;
    t_flags flags = {.dns_resolution = true, .max_wait = 5, .port = 33434, .probe_count = 16};

    // Program needs to be run as root to receive raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &hostname, &flags);

    // Create the sending and receiving sockets
    ft_create_send_socket(&send_packet, hostname);
    ft_create_receive_socket(&receive_packet[0]);

    // Save the IP address of the host
    inet_ntop(AF_INET, &send_packet.addr_in->sin_addr, dest_ip, sizeof(dest_ip));

    // Change the %d value to a 60 byte packet
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", hostname, dest_ip, MAX_TTL, (int)strlen("message"));

    probes = init_probe(&probe_to_send);
    ft_init_results(results);

    while (next_print < final_ttl || !reached) // Change condition to until probes is empty
    {
        if (probe_to_send.ttl <= final_ttl)
            ft_send_packet(&send_packet, probes, &probe_to_send);
        timeout = compute_timeout(probes);
        num_events = poll(receive_packet, 1, (int)timeout);
        if (num_events)
            ft_receive_packet(&receive_packet[0], probes, results, &next_print);
        else
            ft_expired_packet(results, probes, &next_print);
    }
}