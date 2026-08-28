#include "ft_traceroute.h"

/*
 * Resolves the IP address of the given hostname
 */
void resolve_ip(t_host *host)
{
    struct addrinfo hints = {
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_RAW,
                        .ai_protocol = IPPROTO_ICMP},
                    *result;

    // Resolve the DNS, and check if it exists
    if (getaddrinfo(host->hostname, NULL, &hints, &result))
    {
        printf("ft_traceroute: unknown host %s\n", host->hostname);
        exit(1);
    }

    // If it exists, stores the IP address
    struct sockaddr_in *addr = (struct sockaddr_in *)result->ai_addr;
    inet_ntop(AF_INET, &(addr->sin_addr), host->ip_address, INET_ADDRSTRLEN);

    // packet_info->socket_address.sin_family = AF_INET;
    // packet_info->socket_address.sin_addr = addr->sin_addr;

    // Clear the memory space that was allocated for the IP linked list
    freeaddrinfo(result);
}

int main(int ac, char **av)
{
    t_host host;
    int sockfd;

    // Program needs to be run as root to send raw packets
    if (getuid())
    {
        printf("Root permission needed\n");
        return (1);
    }
    ft_parser(ac, av, &host.hostname);
    resolve_ip(&host);
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        printf("ft_traceroute: socket creation failed\n");
        exit(1);
    }
}