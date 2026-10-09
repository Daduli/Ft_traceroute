#include "ft_traceroute.h"

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
    // t_hop_result results[MAX_TTL - START_TTL];

    // Program needs to be run as root to receive raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &hostname);

    // Create the sending and receiving sockets
    ft_create_send_socket(&send_packet, hostname);
    ft_create_receive_socket(&receive_packet[0]);

    // Save the IP address of the host
    inet_ntop(AF_INET, &send_packet.addr_in->sin_addr, dest_ip, sizeof(dest_ip));

    // Change the %d value to a 60 byte packet
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", hostname, dest_ip, MAX_TTL, (int)strlen("message"));

    probes = init_probe(&probe_to_send);

    // printf("Probe to send TTL: %d\n Nb: %d\n", probe_to_send.ttl, probe_to_send.probe_nb);

    // for (int i = 0; i < QUERIES; i++)
    //     printf("Probe[%d] TTL: %d\nPort: %d\nNb: %d\nIn use: %d\n", i, probes[i].ttl, probes[i].port, probes[i].probe_nb, probes[i].in_use);

    // while (probe_to_send.ttl < MAX_TTL)
    // {
    // printf("Loop\n");
    ft_send_packet(&send_packet, probes, &probe_to_send);
    // printf("Timeout for poll: %f\n", compute_timeout(probes));
    timeout = compute_timeout(probes);
    // printf("Poll timeout value: %f\n", timeout);
    num_events = poll(receive_packet, 1, (int)timeout);
    if (num_events)
        ft_receive_packet(&receive_packet[0], probes);
    else
        printf("One packet timed out\n");
    // ft_handle_timeout();
    // }

    // for (int i = 0; i < QUERIES; i++)
    //     printf("Probe[%d] TTL: %d\nPort: %d\nNb: %d\nIn use: %d\n", i, probes[i].ttl, probes[i].port, probes[i].probe_nb, probes[i].in_use);

    //--------------

    // for (int ttl = 1; ttl <= MAX_TTL; ttl++)
    // {
    //     for (int probe = 0; probe < NB_PROBES; probe++)
    //     {
    //         ft_send_packet(&send_packet, &probes[queries], ttl);
    //         queries++;
    //         // printf("Queries value: %d\n", queries);
    //         if (queries == 16)
    //             break;
    //     }
    //     if (queries == 16)
    //     {
    //         // printf("Exited probe loop\n");
    //         queries = 0;
    //         // for (int i = 0; i < QUERIES; i++)
    //         // {
    //         ft_receive_packet(&receive_packet, probes, poll_fd);
    //         // }
    //     }
    // }
}