#include "ft_traceroute.h"

int main(int ac, char **av)
{
    char *hostname;
    char dest_ip[INET_ADDRSTRLEN];
    t_send_packet send_packet;
    struct pollfd receive_packet;

    //----------

    t_probe probes[QUERIES];
    struct pollfd poll_fd[QUERIES];
    int queries = 0;

    // Program needs to be run as root to receive raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &hostname);

    // Create the sending and receiving sockets
    ft_create_send_socket(&send_packet, hostname);
    ft_create_receive_socket(&receive_packet);

    // Save the IP address of the host
    struct sockaddr_in *addr_in = (struct sockaddr_in *)&send_packet.addr_in;
    inet_ntop(AF_INET, &(addr_in->sin_addr), dest_ip, sizeof(dest_ip));

    // Change the %d value to a 60 byte packet
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", hostname, dest_ip, MAX_TTL, (int)strlen("message"));

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