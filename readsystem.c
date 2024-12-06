/* Lukhsaan (elankuml), Jodh (gillj118), Date: December 5, 2024
*
* This program prints the top 5 app that use the most RAM and CPU, gives their stats and prints the battery life of the computer
*/
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <psapi.h>
#include <string.h>
#include "readsystem.h"

//max number of processes program handles
#define MAX_PROCESSES 1024

//represents app with its name and total cpu usage and memory usage in bytes
typedef struct {
    char name[MAX_PATH];    //app name
    double totalCpuUsage;       //cpu usage percentage 
    SIZE_T memoryUsage; // RAM usage in bytes
} CombinedProcess;

//represents individual apps and cpu usage
typedef struct {
    DWORD pid;
    char name[MAX_PATH];
    double cpuUsage;
} ProcessInfo;

/* copmare
* Parameters: 
* const void *a: Pointer to the first element to compare.
* const void *b: Pointer to the second element to compare.
* Return values:  Integer value that determines the relative order of two CombinedProcess structures
* Description: Sorts CombinedProcess structs in descending order of their totalCpuUsage
*/
int compare(const void *a, const void *b) {
    //total cpu usage from combinedProcess structs
    double cpuA = ((CombinedProcess *)a)->totalCpuUsage;
    double cpuB = ((CombinedProcess *)b)->totalCpuUsage;
    return (cpuB > cpuA) - (cpuB < cpuA); // Descending order
}

/* filetime_to_uli
 * Parameters: FILETIME ft: struct containing timestamp 
 * Return values: struct combining low and high parts of FILETIME
 * Description: Converts a `FILETIME` struct into a ULARGE_INTEGER manipulation of time values
 */
ULARGE_INTEGER filetime_to_uli(FILETIME ft) {
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli;
}

/* get_number_of_cores
 * Return values: integer of logical processors
 * Description: Retrieves the number of logical processes on the system
 */
int get_number_of_cores() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

/* get_process_memory_usage
 * Parameters: app ID whose memory usage is to be retrieved
 * Return values: memory usage of app in bytes
 * Description: Gets memory usage or RAM of an app based off its PID number
 */
SIZE_T get_process_memory_usage(DWORD pid) {
    PROCESS_MEMORY_COUNTERS_EX pmc;

    //opening the target app and reading memory
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    //if the app is opened return
    if (hProcess && GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc))) {
        CloseHandle(hProcess);

         // return memory usage in bytes
        return pmc.WorkingSetSize;
    }
    //if app handle is caaid close it
    if (hProcess) {
        CloseHandle(hProcess);
    }
    // if unable to get memory usage
    return 0; 
}

/* get_process_memory_usage
 * Parameters: File pointer used for writing the app info to an output file
 * Description: Collects CPU and RAM  statistics, aggregates the data and sorts them by CPU usage and 
 */
void get_top_processes(FILE *file) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    
    //store CPU times 
    FILETIME idleTime, kernelTime, userTime, prevIdleTime, prevKernelTime, prevUserTime;
    FILETIME prevProcKernelTime[MAX_PROCESSES], prevProcUserTime[MAX_PROCESSES];

    //arrays to store individual and agrragated app datas
    ProcessInfo processes[MAX_PROCESSES];
    CombinedProcess combined[MAX_PROCESSES];
    int processCount = 0, combinedCount = 0;

    //open the file for writing info
    file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
    } 

    //initialize total CPU usage
    double totalCPUUsage = 0.0; 

    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    //calculate total and available RAM in bytes 
    double totalRAM = (double)statex.ullTotalPhys;
    double availableRAM = (double)statex.ullAvailPhys;
    //percentage of used RAM
    double usedRAMPercentage = ((totalRAM - availableRAM) / totalRAM) * 100.0;

    //number of cpu cores
    int numCores = get_number_of_cores();

    // Take an initial system CPU snapshot
    GetSystemTimes(&prevIdleTime, &prevKernelTime, &prevUserTime);
    Sleep(1000); // Allow time for sampling
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    //snapshot of all apps in the system
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("Failed to create process snapshot\n");
        return;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //retrieve the first app in snapshot
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Failed to retrieve process list\n");
        CloseHandle(hProcessSnap);
        return;
    }

    // Collect initial per-process times
    do {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess && processCount < MAX_PROCESSES) {
            FILETIME createTime, exitTime, procKernelTime, procUserTime;
            if (GetProcessTimes(hProcess, &createTime, &exitTime, &procKernelTime, &procUserTime)) {
                processes[processCount].pid = pe32.th32ProcessID;
                //store app name
                strncpy(processes[processCount].name, pe32.szExeFile, MAX_PATH);
                //store initial kernel and times
                prevProcKernelTime[processCount] = procKernelTime;
                prevProcUserTime[processCount] = procUserTime;
                processCount++;
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);

    // take a second snapshot for per-app CPU usage calculation
    Sleep(1000); // allow time for per-app sampling
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return;
    }

    int i = 0;
    do {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess && i < processCount) {
            FILETIME createTime, exitTime, procKernelTime, procUserTime;
            if (GetProcessTimes(hProcess, &createTime, &exitTime, &procKernelTime, &procUserTime)) {
                ULARGE_INTEGER prevKernel, prevUser, currKernel, currUser;
                ULARGE_INTEGER prevSysIdle, prevSysKernel, prevSysUser, currSysIdle, currSysKernel, currSysUser;

                //convert FILETIME to ULARGE_INTEGER for calclations
                prevKernel = filetime_to_uli(prevProcKernelTime[i]);
                prevUser = filetime_to_uli(prevProcUserTime[i]);
                currKernel = filetime_to_uli(procKernelTime);
                currUser = filetime_to_uli(procUserTime);

                prevSysIdle = filetime_to_uli(prevIdleTime);
                prevSysKernel = filetime_to_uli(prevKernelTime);
                prevSysUser = filetime_to_uli(prevUserTime);

                currSysIdle = filetime_to_uli(idleTime);
                currSysKernel = filetime_to_uli(kernelTime);
                currSysUser = filetime_to_uli(userTime);

                //calculate deltas
                ULONGLONG procTimeDelta = (currKernel.QuadPart - prevKernel.QuadPart) +
                                          (currUser.QuadPart - prevUser.QuadPart);
                ULONGLONG sysTimeDelta = (currSysKernel.QuadPart - prevSysKernel.QuadPart) +
                                         (currSysUser.QuadPart - prevSysUser.QuadPart);

                //validate deltas
                if (procTimeDelta > sysTimeDelta || sysTimeDelta == 0 || procTimeDelta > 10 * sysTimeDelta) {
                    // Invalid delta: skip this app
                    processes[i].cpuUsage = 0.0;
                } else {
                    //normalize CPU usage
                    processes[i].cpuUsage = (100.0 * procTimeDelta) / (sysTimeDelta * numCores);
                }
            }
            CloseHandle(hProcess);
        }
        i++;
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);

    //combine apps with the same name and aggregate CPU and memory usage.
    for (int i = 0; i < processCount; i++) {
        if (processes[i].cpuUsage > 0) { // Ignore invalid processes
            int found = 0;
            for (int j = 0; j < combinedCount; j++) {
                if (strcmp(processes[i].name, combined[j].name) == 0) {
                    combined[j].totalCpuUsage += processes[i].cpuUsage;
                    combined[j].memoryUsage += get_process_memory_usage(processes[i].pid); // Sum memory usage
                    found = 1;
                    break;
                }
            }
            if (!found && combinedCount < MAX_PROCESSES) {
                strncpy(combined[combinedCount].name, processes[i].name, MAX_PATH);
                combined[combinedCount].totalCpuUsage = processes[i].cpuUsage;
                combined[combinedCount].memoryUsage = get_process_memory_usage(processes[i].pid);
                totalCPUUsage += processes[i].cpuUsage;
                combinedCount++;
            }
        }
    }

    //sort combined apps by total CPU usage
    qsort(combined, combinedCount, sizeof(CombinedProcess), compare);

    //print output in requested format
    for (int i = 0; i < combinedCount && i < 5; i++) {
       fprintf(file, "%s\n", combined[i].name);
    }
    //write the top 5 apps and system statistics to the output file.
    for (int i = 0; i < combinedCount && i < 5; i++) {
        fprintf(file, "%.2f%%\n", combined[i].totalCpuUsage);
    }
    for (int i = 0; i < combinedCount && i < 5; i++) {
        fprintf(file, "%.2f MB\n", combined[i].memoryUsage / (1024.0 * 1024.0));
    }
    
    //totals
    fprintf(file, "%.2f%%\n", totalCPUUsage);
    fprintf(file, "%.2f%%\n", usedRAMPercentage);
    fprintf(file, "Total Usage\n");
    fprintf(file, "Battery Life\n");
    print_battery_status(file);
    //close output file
    fclose(file);    
}

/* print_battery_status
 * Parameters: File pointer used for writing the app info to an output file
 * Description: Write battery life to file
 */
void print_battery_status(FILE *file) {
    SYSTEM_POWER_STATUS status;
    //retrieve current battery
    if (GetSystemPowerStatus(&status)) {
        fprintf(file, "%d%%\n", status.BatteryLifePercent);
    } else { //if it cant be retrieved
        printf("Error retrieving battery status.\n");
    }
}
