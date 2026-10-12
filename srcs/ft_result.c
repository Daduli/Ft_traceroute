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
        printf("Hop nb %d is printable\n", results[*next_print].ttl);
        (*next_print)++;
    }
}

// Takes the one probe that has resolved (reply or timeout) 3 times
// Takes the ip and hostname of the sender
// Set a TTl to the table in the array
// Initial record array need to be initialized
// One function for probe res and one for hop res
// Needs a way to know which hop to print in the correct order
void ft_record_result(t_hop_result *results, t_probe *probe, struct sockaddr_in *addr_in, float rtt, bool replied, int *next_print)
{
    // Call this for resolved
    // If the table is not init, do the init
    // Else skip to the RTT saving
    // printf("Recording result, TTL: %d - Probe nb: %d\n", results[probe->ttl - 1].ttl, probe->probe_nb);

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
    } // Print line
}