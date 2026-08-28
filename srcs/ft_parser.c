#include "../ft_traceroute.h"

/*
 * Returns the type of the given argument as:
 * 'D' = Double dash flag (--help)
 * 'S' = Single dash flag
 * 'H' = Hostname or IP address
 */
char get_argument_type(char *argument)
{

    if (argument[0] == '-')
    {
        if (argument[1] == '-')
            return ('D');
        return ('S');
    }
    return ('H');
}

/*
 * Parses the command line arguments
 */
void ft_parser(int ac, char **av, char **hostname)
{
    int host_count = 0;

    for (int i = 1; i < ac; i++)
    {
        char type = get_argument_type(av[i]);
        if (type == 'H')
        {
            *hostname = av[i];
            host_count++;
        }
        else if (type == 'D')
            print_help();
        else if (type == 'S')
            ; // Add single dash flag handling logic here
    }

    if (host_count < 1)
    {
        printf("ft_traceroute: missing host operand\n"
               "Try 'ft_traceroute --help' for more information.\n");
        exit(1);
    }
    else if (host_count > 1)
    {
        printf("ft_traceroute: too many host operands\n");
        exit(1);
    }
}