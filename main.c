/* Lukhsaan (elankuml), Jodh (gillj118), Date: December 5, 2024
*
* This program uses the readsystems file to print cpu usages and ram of the top 5 apps that use them the most and battery life
*/
#include "readsystem.h"
#include <stdio.h>
#include <string.h>


/* filetime_to_uli
 * Parameters: --help or none
 * Return values: 1 or 0
 * Description: same as program
 */
int main(int argc, char *argv[]) {
    //if arguement is --help
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        char help[] =
            "Usage: readsystem\n"
            "Outputs CPU usage of top 5 apps and RAM usage.\n"
            "--help: Displays this text.\n";
        printf("%s", help);
    } else { //get all the stats
        get_top_processes();  
    }
    return 0;
}
