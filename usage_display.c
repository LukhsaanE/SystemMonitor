#include <gtk/gtk.h>


//Calculates the totat percent usage of a specific part of the system
int calculate_total_usage(int argc, char *argv[]) {
  int total = 0;
  for (int i = 0;i < argc; i++) {
    total += *argv[i];
  }
  return total;
}

//Prints the --help statement to the console.
static void print_help (GtkWidget *widget, gpointer   data)
{
  g_print("System Monitoring Tool v0.1.0\n");
  g_print ("This application displays the system percent usage of the top 5 appliations currently running\n");
  g_print ("Usage: ./hello\n");
  g_print ("This application requires no arguments. It gets all needed data directly from the system.\n");
}

//This method holds the GUI display code
static void activate (GtkApplication* app,
          gpointer        user_data)
{
    GtkWidget *window; //initializes the window
    GtkWidget *grid; //initializes the grid to keep displayed information organized
		     
//Holds the name of the 5 apps being displayed on the monitering tool
    GtkWidget *App1;
    GtkWidget *App2;
    GtkWidget *App3;
    GtkWidget *App4;
    GtkWidget *App5;

//Holds the total % use of a piece of hardware
    GtkWidget *Total;

//Displays the peices of hardware
    GtkWidget *CPU;
    GtkWidget *GPU;
    GtkWidget *RAM;
    GtkWidget *Battery;
    GtkWidget *Spacer;

//Each App's percent usage of the CPU
    GtkWidget *CPU_App1;
    GtkWidget *CPU_App2;
    GtkWidget *CPU_App3;
    GtkWidget *CPU_App4;
    GtkWidget *CPU_App5;


//Create's the help button
    GtkWidget *button;

    // Create the main application window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grid Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

    // Create a grid layout
    grid = gtk_grid_new();

    // Create a label the displayed text
    App1 = gtk_label_new("\nApp 1");
    App2 = gtk_label_new("\nApp 2");
    App3 = gtk_label_new("\nApp 3");
    App4 = gtk_label_new("\nApp 4");
    App5 = gtk_label_new("\nApp 5");
    Total = gtk_label_new("\nTotal Usage");
    CPU = gtk_label_new("\nCPU Usage\t");
    GPU = gtk_label_new("\nGPU Usage\t");
    RAM = gtk_label_new("\nRAM Usage\t");
    Battery = gtk_label_new("\nBattery Usage\t");
    Spacer = gtk_label_new("\n\t");

    CPU_App1 = gtk_label_new("\n100%\t");
    CPU_App2 = gtk_label_new("\n0%\t");
    CPU_App3 = gtk_label_new("\n0%\t");
    CPU_App4 = gtk_label_new("\n0%\t");
    CPU_App5 = gtk_label_new("\n0%\t");

    //Creates and lebels the help button
    button = gtk_button_new_with_label("Help");
    g_signal_connect (button, "clicked", G_CALLBACK (print_help), NULL);

    // Add widgets to specific rows and columns in the grid
    //Displays Apps in a column
    gtk_grid_attach(GTK_GRID(grid), App1, 1, 1, 1, 1);  // Place label at (1, 1)
    gtk_grid_attach(GTK_GRID(grid), App2, 1, 2, 1, 1);  // Place label at (1, 2)
    gtk_grid_attach(GTK_GRID(grid), App3, 1, 3, 1, 1);  // Place label at (1, 3)
    gtk_grid_attach(GTK_GRID(grid), App4, 1, 4, 1, 1);  // Place label at (1, 4)
    gtk_grid_attach(GTK_GRID(grid), App5, 1, 5, 1, 1);  // Place label at (1, 5)

    gtk_grid_attach(GTK_GRID(grid), Total, 1, 6, 1, 1);
    //Puts hardware in a row
    gtk_grid_attach(GTK_GRID(grid), CPU, 2, 0, 1, 1);  // Place label at (2, 0)
    gtk_grid_attach(GTK_GRID(grid), GPU, 3, 0, 1, 1);  // Place label at (3, 0)
    gtk_grid_attach(GTK_GRID(grid), RAM, 4, 0, 1, 1);  // Place label at (4, 0)
    gtk_grid_attach(GTK_GRID(grid), Battery, 5, 0, 1, 1);  // Place label at (5, 0)
    gtk_grid_attach(GTK_GRID(grid), Spacer, 6, 0, 1, 1);  // Place label at (6, 0)
    //Puts CPU usage in one column under CPU label
    gtk_grid_attach(GTK_GRID(grid), CPU_App1, 2, 1, 1, 1);  // Place label at (2, 1)
    gtk_grid_attach(GTK_GRID(grid), CPU_App2, 2, 2, 1, 1);  // Place label at (2, 2)
    gtk_grid_attach(GTK_GRID(grid), CPU_App3, 2, 3, 1, 1);  // Place label at (2, 3)
    gtk_grid_attach(GTK_GRID(grid), CPU_App4, 2, 4, 1, 1);  // Place label at (2, 4)
    gtk_grid_attach(GTK_GRID(grid), CPU_App5, 2, 5, 1, 1);  // Place label at (2, 5)

    //Places the help button
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1); 

    // Set the grid as the child of the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Show the window
    gtk_window_present(GTK_WINDOW(window));
}

//Main Function
int main(int argc, char **argv)
{
    GtkApplication *app; //GtkApplication object
    int status;

    // Create a new GtkApplication instance
    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    
    //connects the acivtate signal to the activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    //initializes application and runs window program
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

