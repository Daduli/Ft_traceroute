#include "../ft_traceroute.h"

void ft_init_results(t_hop_result *results)
{
    for (int i = 0; i < 30; i++)
    {
        results[i].ttl = i + 1;
        results[i].replies = 0;
        results[i].printable = false;
    }
}

void ft_print_result(t_hop_result *results, int *next_print)
{
    while (results[*next_print].printable && *next_print < MAX_TTL)
    {
        char src_ip[INET_ADDRSTRLEN];
        printf("%d  ", results[*next_print].ttl);
        for (int i = 0; i < PROBE_PER_HOP; i++)
        {
            if (results[*next_print].res[i].replied)
            {
                inet_ntop(AF_INET, &results[*next_print].res[i].addr_in->sin_addr, src_ip, INET_ADDRSTRLEN);
                printf("%s %.3f ms ", src_ip, results[*next_print].res[i].rtt);

                // Get the sender's hostname
                // getnameinfo((struct sockaddr *)addr_in, addr_len, sender_hostname, 256, NULL, 0, 0);
                // printf("Hostname : %s\n", sender_hostname);
            }
            else
            {
                printf(" * ");
            }
        }
        printf("\n");
        (*next_print)++;
    }
}

void ft_record_result(t_hop_result *results, t_probe *probe, struct sockaddr_in *addr_in, float rtt, bool replied, int *next_print)
{
    if (replied)
    {
        results[probe->ttl - 1].res[probe->probe_nb].rtt = rtt;
        results[probe->ttl - 1].res[probe->probe_nb].addr_in = addr_in;
    }
    results[probe->ttl - 1].res[probe->probe_nb].replied = replied;
    results[probe->ttl - 1].replies++;

    if (results[probe->ttl - 1].replies == 3)
    {
        results[probe->ttl - 1].printable = true;
        ft_print_result(results, next_print);
    }
}