#include "ft_traceroute.h"

int main(int ac, char **av)
{
    char *hostname;
    char host_ip[INET_ADDRSTRLEN];
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

    ft_send_packet(&send_packet, 1);
}