#include "../ft_traceroute.h"

// Takes the one probe that has resolved (reply or timeout) 3 times
// Takes the ip and hostname of the sender
// Set a TTl to the table in the array
// Initial record array need to be initialized
// One function for probe res and one for hop res
// Needs a way to know which hop to print in the correct order
void ft_record_result(t_probe *probe, bool replied)
{
    // Call this for resolved
    // If the table is not init, do the init
    // Else skip to the RTT saving
}