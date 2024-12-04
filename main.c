#include "readsystem.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        char help[] =
            "Usage: readsystem\n"
            "Outputs CPU usage of top 5 apps and RAM usage.\n"
            "--help: Displays this text.\n";
        printf("%s", help);
    } else {
        get_top_cpu_processes(); 
        print_battery_status();  
    }
    return 0;
}

