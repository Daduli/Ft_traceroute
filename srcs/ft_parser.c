#include "../ft_traceroute.h"

/*
 * Returns the type of the given argument as:
 * 'D' = Double dash flag (--help)
 * 'S' = Single dash flag
 * 'H' = Hostname or IP address
 */
char ft_get_argument_type(char *argument)
{

    if (argument[0] == '-')
    {
        if (argument[1] == '-')
            return (DOUBLE_DASH);
        return (SINGLE_DASH);
    }
    return (HOSTNAME);
}

char ft_get_flags(char *option)
{
    if (*option == 'p')
        return (PORT);
    else if (*option == 'n')
        return (DNS_RESOLUTION);
    else if (*option == 'N')
        return (PROBE_COUNT);
    else if (*option == 'w')
        return (WAIT_TIME);
    return (0);
}

void ft_save_option(t_flags *flags, char *argument, char flag_type)
{
    printf("Saving option argument: %d\n", atoi(argument));

    for (int i = 0; argument[i]; i++)
    {
        if (!isdigit(argument[i]))
        {
            printf("ft_traceroute: Error on option argument, not a digit\n");
            exit(1);
        }
    }

    int opt_arg = atoi(argument);
    if (flag_type == PORT)
    {
        if (opt_arg <= 65535)
            flags->port = (uint16_t)opt_arg;
        else
        {
            printf("ft_traceroute: Port number too big\n");
            exit(1);
        }
    }
    else if (flag_type == PROBE_COUNT)
        flags->probe_count = opt_arg;
    else if (flag_type == WAIT_TIME)
        flags->max_wait = opt_arg;
}

/*
 * Parses the command line arguments and saves the hostname in the provided pointer.
 */
void ft_parser(int ac, char **av, char **hostname, t_flags *flags)
{
    int host_count = 0;
    char flag_type;
    bool has_flag = false;

    for (int i = 1; i < ac; i++)
    {
        char type = ft_get_argument_type(av[i]);
        if (type == HOSTNAME)
        {
            *hostname = av[i];
            host_count++;
        }
        else if (type == DOUBLE_DASH)
            print_help();
        else if (type == SINGLE_DASH)
        {
            has_flag = true;
            flag_type = ft_get_flags(av[i] + 1); // Add single dash flag handling logic here
            if (!flag_type)
            {
                printf("ft_traceroute: Invalid flag\n");
                exit(1);
            }
            else if (flag_type != DNS_RESOLUTION && !av[i + 1])
            {
                printf("ft_traceroute: No argument found\n");
                exit(1);
            }
            else if (flag_type != DNS_RESOLUTION)
                ft_save_option(flags, av[++i], flag_type);
            else
                flags->dns_resolution = false;
        }
    }

    if (host_count < 1 && has_flag)
    {
        printf("ft_traceroute: Specify \"host\" missing argument.\n");
        exit(1);
    }
    else if (host_count < 1)
        print_help();
    else if (host_count > 1)
    {
        printf("ft_traceroute: too many host operands\n");
        exit(1);
    }
}