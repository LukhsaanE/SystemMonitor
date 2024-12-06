/*
This c program displays the computers CPU, RAM usage of the top 5 applications and the battery percentage of the system for a windows computer.
This uses GTK4 to display a graphic user interface.

Ranvir Jhajj, jhajjr1, 400524715
Garrick Wong, wongg36, 400518507
*/


#include <gtk/gtk.h>
#include <stdlib.h>
#define FILENAME_SIZE 1024
#define MAX_LINE 2048
#include <stdbool.h>
#include "readsystem.h"

//Prints the --help statement to the console.
//Takes widget as the parameter and prints the help statement
static void print_help (GtkWidget *widget)
{ 
  g_print("System Monitoring Tool v0.1.0\n");
  g_print ("This application displays the system percent usage of the top 5 appliations currently running\n");
  g_print ("Usage: ./hello\n");
  g_print ("This application requires no arguments. It gets all needed data directly from the system.\n");
}



// Simulated usage update function
// This function takes parameters for all the labels that require updating
// Runs the get_top_processes(file) function to write into the output.txt file with all the needed data
// Updates all the labels from the file
void update_usage(GtkWidget *label1, GtkWidget *label2, GtkWidget *label3, GtkWidget *label4,GtkWidget *label5,
GtkWidget *label6, GtkWidget *label7, GtkWidget *label8, GtkWidget *label9, GtkWidget *label10,
GtkWidget *label11, GtkWidget *label12, GtkWidget *label13, GtkWidget *label14, GtkWidget *label15, GtkWidget *label16, GtkWidget *label17,
GtkWidget *label18, GtkWidget *label19, GtkWidget *label20) {
    FILE *file;
    get_top_processes(file);
    char buffer[MAX_LINE];
    GtkWidget *labels[] = {label1, label2, label3, label4, label5, label6, label7, label8, label9, 
    label10, label11, label12, label13, label14, label15, label16, label17, label18, label19, label20};
    int target_lines[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}; // Target lines for each label
    int current_line = 0;
    int target_index = 0;

    // Open the file
    file = fopen("output.txt", "r");
    if (file == NULL) {
        printf("Error Opening File.\n");
        return;
    }

    // Iterate through the file line by line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        current_line++;

        // Check if the current line matches any target line
        if (current_line == target_lines[target_index]) {


            // Update the corresponding label if it exists
            if (labels[target_index]) {
                gtk_label_set_text(GTK_LABEL(labels[target_index]), buffer);
            }

            // Move to the next target line
            target_index++;
            if (target_index >= 20) {
                break; // Exit if all labels have been updated
            }
        }
    }

    // Close the file
    fclose(file);
    // Set remaining labels to a default value if no corresponding line exists
    for (int i = target_index; i < 5; i++) {
        if (labels[i]) {
            gtk_label_set_text(GTK_LABEL(labels[i]), "N/A");
        }
    }
}

//This functoin takes the gpointer data as the parameter and assigns it to the labels array
//Then runs update_usage() with all the labels.
gboolean on_update_data(gpointer data) {
    GtkWidget **labels = (GtkWidget **)data;

    // Ensure labels array is valid
    if (!labels) return FALSE;

    // Update the usage labels
    update_usage(labels[0], labels[1], labels[2], labels[3], labels[4],labels[5], labels[6], labels[7], labels[8], labels[9],
    labels[10], labels[11], labels[12], labels[13], labels[14], labels[15], labels[16], labels[17], labels[18], labels[19]);

    return TRUE;
}

//Takes parameter for the application and data.
//Initializes all the labels and buttons needed for the GUI
//Attaches everything to the grid to form the layout
//Assigns each index in labels array to the corresponding label to be run in update_usage()
//It then runs the on_update() using g_timeout_add which runs it every 2100 ms (2.1 seconds)

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window, *grid;
    GtkWidget *App1, *App2, *App3, *App4, *App5, *Total, *Battery;
    GtkWidget *CPU_App1, *CPU_App2, *CPU_App3, *CPU_App4, *CPU_App5;
    GtkWidget *RAM_App1, *RAM_App2, *RAM_App3, *RAM_App4, *RAM_App5;
    GtkWidget *CPU, *RAM, *Spacer;
    GtkWidget *button;
    GtkWidget *TotalCPU, *TotalRAM, *BatteryPercent;


    // Create the main application window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "System Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    // Create a grid layout
    grid = gtk_grid_new();

    // Create labels
    App1 = gtk_label_new("\nApp 1");
    App2 = gtk_label_new("\nApp 2");
    App3 = gtk_label_new("\nApp 3");
    App4 = gtk_label_new("\nApp 4");
    App5 = gtk_label_new("\nApp 5");
    Total = gtk_label_new("\nTotal Usage");
    Battery = gtk_label_new("\nBattery");

    CPU = gtk_label_new("\nCPU Usage\t");
    RAM = gtk_label_new("\nRam Usage\t");
    Spacer = gtk_label_new("\n\t");

    CPU_App1 = gtk_label_new("0%");
    CPU_App2 = gtk_label_new("0%");
    CPU_App3 = gtk_label_new("0%");
    CPU_App4 = gtk_label_new("0%");
    CPU_App5 = gtk_label_new("0%");
    TotalCPU = gtk_label_new("0%");


    RAM_App1 = gtk_label_new("0%");
    RAM_App2 = gtk_label_new("0%");
    RAM_App3 = gtk_label_new("0%");
    RAM_App4 = gtk_label_new("0%");
    RAM_App5 = gtk_label_new("0%");
    TotalRAM = gtk_label_new("0%");
    BatteryPercent = gtk_label_new("0%");



    //Creates and labels the help button
    button = gtk_button_new_with_label("Help");
    g_signal_connect (button,"clicked",G_CALLBACK(print_help), NULL);

    // Attach widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), App1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App4, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App5, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Total, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), Battery, 1, 7, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), CPU, 2,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), RAM, 4,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), Spacer, 6,0,1,1);    

    gtk_grid_attach(GTK_GRID(grid), CPU_App1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App3, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App4, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App5, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TotalCPU, 2, 6, 1, 1);


    gtk_grid_attach(GTK_GRID(grid), RAM_App1, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), RAM_App2, 4, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), RAM_App3, 4, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), RAM_App4, 4, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), RAM_App5, 4, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TotalRAM, 4, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), BatteryPercent, 2, 7, 1, 1);



    gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);

    // Set the grid as the child of the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Create labels array on the heap
    GtkWidget **labels = g_new(GtkWidget *, 20);
    labels[0] = App1;
    labels[1] = App2;
    labels[2] = App3;
    labels[3] = App4;
    labels[4] = App5;
    labels[5] = CPU_App1;
    labels[6] = CPU_App2;
    labels[7] = CPU_App3;
    labels[8] = CPU_App4;
    labels[9] = CPU_App5;
    labels[10] = RAM_App1;
    labels[11] = RAM_App2;
    labels[12] = RAM_App3;
    labels[13] = RAM_App4;
    labels[14] = RAM_App5;
    labels[15] = TotalCPU;
    labels[16] = TotalRAM;
    labels[17] = Total;
    labels[18] = Battery;
    labels[19] = BatteryPercent;

        // Schedule periodic updates
    g_timeout_add(2100, on_update_data, labels);
        // Show the window
    gtk_window_present(GTK_WINDOW(window)); 

}

//starts the app and allows the rest of the functions to run
//takes argv and argc as arguments
//returns status of the application
int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_REPLACE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
