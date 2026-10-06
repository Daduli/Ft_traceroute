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

/*
 * Go through all the probes sent and find the earliest one to timeout
 * Returns the earliest timeout
 */
float compute_timeout(t_probe *probes)
{
    float deadline_ms, earliest_deadline = INFINITY;
    float now_ms;
    struct timespec now;

    for (int i = 0; i < QUERIES; i++)
    {
        if (probes[i].in_use)
        {
            deadline_ms = probes[i].send_time.tv_sec * 1000 + probes[i].send_time.tv_nsec / 1000000 + WAIT_TIME_MS;
            // printf("Deadline for probe[%d]: %f\n", i, deadline_ms);
            if (deadline_ms < earliest_deadline)
                earliest_deadline = deadline_ms;
        }
    }

    // If no deadline find, but shouldn't happen
    if (earliest_deadline == INFINITY)
        return (-1.0f);

    clock_gettime(CLOCK_MONOTONIC, &now);
    now_ms = now.tv_sec * 1000 + now.tv_nsec / 1000000;
    // printf("Now in ms: %f\n", now_ms);

    return ((earliest_deadline - now_ms ? earliest_deadline - now_ms : 0));
}