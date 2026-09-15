#include "ft_traceroute.h"

int main(int ac, char **av)
{
    char *hostname;
    char destination_ip[INET_ADDRSTRLEN];
    t_packet send_packet;
    t_probe probes[QUERIES];

    // Program needs to be run as root to receive raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &hostname);

    ft_sending_socket(&send_packet, hostname);

    for (int i = 0; i < QUERIES; i++)
    {
        ft_receiving_socket(&probes[i]);
        printf("Receiving socket fd: %d\n", probes[i].poll_fd.fd);
        printf("Receiving socket events: %d\n", probes[i].poll_fd.events);
    }
    // Save the IP address of the host
    struct sockaddr_in *addr_in = (struct sockaddr_in *)&send_packet.address_infos;
    inet_ntop(AF_INET, &(addr_in->sin_addr), destination_ip, sizeof(destination_ip));

    printf("IP address of %s: %s\n", hostname, destination_ip);

    // printf("traceroute to %s (%s), %d hops max, %d byte packets\n", hostname, destination_ip, MAX_TTL, (int)strlen("message"));

    // for (int ttl = 1; ttl <= MAX_TTL; ttl++)
    // {
    //     printf("%2d  ", ttl);
    //     for (int probe = 0; probe < NB_PROBES; probe++)
    //     {
    //         ft_send_packet(&send_packet, ttl);
    //         ft_receive_packet(&receive_packet);
    //     }
    // }
}