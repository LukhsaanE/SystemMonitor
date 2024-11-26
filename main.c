#include <stdio.h>
#include <string.h>
#include "readsystem.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        char help[] =
            "The --help command\n"
            "Usage: readsystem\n\n"
            "Outputs CPU usage of top 5 apps and RAM usage\n\n"
            "--help\tDisplays this text\n\n"
            "Examples: \n"
            "readsystem\tOutputs all the CPU and RAM stats\n";
        printf("%s", help);
    } else {
        get_top_cpu_processes();
    }
    return 0;
}
