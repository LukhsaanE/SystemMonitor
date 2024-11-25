#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ram_reading.h"
#include "readsystem.h"

int main(int argc, char *argv[])
{
	if (argc == 2 && (strcmp(argv[1], "--help") == 0))
	{
		 char help[] =
        	"The --help command\n"
        	"Usage: read \n\n"
        	"Outputs CPU usage of top 5 apps and RAM usage\n\n"
       	 	"--help\tcurrent text\n\n"
        	"Examples: \n"
        	"read\t outputs all the CPU and RAM stats\n";
        	printf("%s", help);	
	}
	
	else
	{
		printMemoryInGB();
		get_top_cpu_processes();
	}
}
