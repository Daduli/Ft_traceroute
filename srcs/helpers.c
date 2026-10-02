#include "../ft_traceroute.h"

/*
 * Prints the help message
 */
void print_help()
{
    printf("Usage: ./ft_traceroute [OPTIONS...] HOST\n"
           "Tracks the route packets take to a network host.\n\n"
           "Options:\n"
           "  --help\t\tRead this help and exit\n");
    exit(0);
}

/*
 * Initialize the data for the probes to be sent
 * Returns an array of t_probe of the number of probes to be sent out simultaneously
 */
t_probe *init_probe(t_cursor *probe_to_send)
{
    probe_to_send->ttl = START_TTL;
    probe_to_send->probe_nb = 0;
    probe_to_send->port = 0;
    return ((t_probe *)calloc(sizeof(t_probe), (ul)QUERIES));
}