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
t_probe *init_probe(t_cursor *probe_to_send, size_t probe_nb)
{
    probe_to_send->ttl = START_TTL;
    probe_to_send->probe_nb = 0;
    probe_to_send->port = 0;
    return ((t_probe *)calloc(sizeof(t_probe), probe_nb));
}

/*
 * Go through all the probes sent and find the earliest one to timeout
 * Returns the earliest timeout
 */
float compute_timeout(t_probe *probes, t_hop_result *results, t_flags flags)
{
    float deadline_ms = 0, earliest_deadline = INFINITY;
    float now_ms;
    struct timespec now;

    for (int i = 0; i < flags.probe_count; i++)
    {
        if (probes[i].in_use)
        {
            // clock_gettime(CLOCK_MONOTONIC, &now);
            // now.tv_sec -= probes[i].send_time.tv_sec;
            // now.tv_nsec -= probes[i].send_time.tv_nsec;
            // float time_ms = (float)now.tv_sec * 1000 + (float)now.tv_nsec / 1000000;

            // printf("Diff time: %.3f\n", time_ms);

            for (int j = probes[i].ttl; j <= MAX_TTL; j++)
            {
                if (j == MAX_TTL)
                    earliest_deadline = flags.max_wait;
                else if (results[j - 1].rtt != INFINITY)
                {
                    earliest_deadline = results[j - 1].rtt;
                    break;
                }
            }
            printf("First RTT found: %.3f\n", earliest_deadline);

            // clock_gettime(CLOCK_MONOTONIC, &now);
            // now.tv_sec -= probes[i].send_time.tv_sec;
            // now.tv_nsec -= probes[i].send_time.tv_nsec;
            // deadline_ms = (float)now.tv_sec * 1000 + (float)now.tv_nsec / 1000000;

            // if (deadline_ms < earliest_deadline)
            //     earliest_deadline = deadline_ms;
        }
    }

    // If no deadline find, but shouldn't happen
    if (earliest_deadline == INFINITY)
        return (-1.0f);

    return (5.0f);

    // return ((earliest_deadline - now_ms > 0 ? earliest_deadline - now_ms : 0));
}