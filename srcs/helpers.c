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