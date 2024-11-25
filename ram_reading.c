#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printMemoryInGB() 
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL) 
    {
        perror("Error opening /proc/meminfo");
        return;
    }

    char line[256];
    long long memTotal = 0, memAvailable = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        if (sscanf(line, "MemTotal: %lld kB", &memTotal) == 1 || sscanf(line, "MemAvailable: %lld kB", &memAvailable) == 1) 
	{
        }
    }
    fclose(file);

    double totalGB = memTotal / (1024.0 * 1024.0);
    double availableGB = memAvailable / (1024.0 * 1024.0);
    double usedGB = totalGB - availableGB;

    printf("Total RAM: %.2f GB\n", totalGB);
    printf("Available RAM: %.2f GB\n", availableGB);
    printf("Used RAM: %.2f GB\n", usedGB);
}
