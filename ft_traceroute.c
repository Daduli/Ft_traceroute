#include "ft_traceroute.h"

int main(int ac, char **av)
{
    char *hostname;
    char host_ip[INET_ADDRSTRLEN];
    int received = 0;
    t_send_packet send_packet;
    t_receive_packet receive_packet;

    // Program needs to be run as root to receive raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &hostname);

    ft_sending_socket(&send_packet, hostname);
    ft_receiving_socket(&receive_packet);

    // Save the IP address of the host
    struct sockaddr_in *addr = (struct sockaddr_in *)&send_packet.address;
    inet_ntop(AF_INET, &(addr->sin_addr), host_ip, INET_ADDRSTRLEN);

    printf("traceroute to %s (%s), %d hops max, %d byte packets\n", hostname, host_ip, MAX_TTL, (int)strlen("message"));

    for (int ttl = 1; ttl <= MAX_TTL; ttl++)
    {
        printf("%2d  ", ttl);
        for (int probe = 0; probe < NB_PROBES; probe++)
        {
            ft_send_packet(&send_packet, ttl);
            ft_receive_packet(&receive_packet);
        }
    }
}