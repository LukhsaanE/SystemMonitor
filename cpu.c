#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PROCESSES 1024

typedef struct {
    int pid;
    char name[256];
    float cpu_usage;
} ProcessInfo;

int get_cpu_cores() {
    return sysconf(_SC_NPROCESSORS_ONLN); // Get number of CPU cores
}

void read_cpu_times(long *total_time, long *idle_time) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    long user, nice, system, idle, iowait, irq, softirq, steal;
    sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld", 
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    *total_time = user + nice + system + idle + iowait + irq + softirq + steal;
    *idle_time = idle + iowait;
}

long read_process_time(int pid) {
    char path[256], buffer[512];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *file = fopen(path, "r");
    if (!file) return -1;

    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    long utime, stime;
    sscanf(buffer, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %ld %ld",
           &utime, &stime);

    return utime + stime; // Total process time
}

int get_all_processes(ProcessInfo processes[], int max_processes) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Failed to open /proc");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);

            char path[256], name[256];
            snprintf(path, sizeof(path), "/proc/%d/stat", pid);

            FILE *file = fopen(path, "r");
            if (!file) continue;

            if (fscanf(file, "%*d (%[^)])", name) == 1) {
                processes[count].pid = pid;
                strncpy(processes[count].name, name, sizeof(processes[count].name));
                processes[count].cpu_usage = 0.0;
                count++;
                if (count >= max_processes) break;
            }
            fclose(file);
        }
    }
    closedir(dir);
    return count;
}

int compare_cpu_usage(const void *a, const void *b) {
    ProcessInfo *p1 = (ProcessInfo *)a;
    ProcessInfo *p2 = (ProcessInfo *)b;
    return (p2->cpu_usage - p1->cpu_usage) > 0 ? 1 : -1;
}

int main() {
    ProcessInfo processes[MAX_PROCESSES];
    int process_count = get_all_processes(processes, MAX_PROCESSES);

    long total_time_start, total_time_end, idle_time;
    int cpu_cores = get_cpu_cores();

    // Read initial total CPU time
    read_cpu_times(&total_time_start, &idle_time);

    // Read initial process times
    for (int i = 0; i < process_count; i++) {
        processes[i].cpu_usage = read_process_time(processes[i].pid);
    }

    sleep(1); // Interval to calculate CPU usage

    // Read final total CPU time
    read_cpu_times(&total_time_end, &idle_time);

    long total_diff = total_time_end - total_time_start;

    // Read final process times and calculate usage
    for (int i = 0; i < process_count; i++) {
        long process_time_end = read_process_time(processes[i].pid);
        if (process_time_end >= 0) {
            long process_diff = process_time_end - processes[i].cpu_usage;
            processes[i].cpu_usage = ((float)process_diff / total_diff) * 100.0 / cpu_cores;
        } else {
            processes[i].cpu_usage = 0.0; // Process might have terminated
        }
    }

    // Sort processes by CPU usage
    qsort(processes, process_count, sizeof(ProcessInfo), compare_cpu_usage);

    printf("Top 5 Processes by CPU Usage:\n");
    for (int i = 0; i < 5 && i < process_count; i++) {
        if (processes[i].cpu_usage > 0) {
            printf("PID: %d, Name: %s, CPU Usage: %.2f%%\n", 
                   processes[i].pid, processes[i].name, processes[i].cpu_usage);
        }
    }

    return 0;
}

