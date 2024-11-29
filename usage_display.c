#include <gtk/gtk.h>
#include <stdlib.h>

//Prints the --help statement to the console.
static void print_help (GtkWidget *widget, gpointer   data)
{ 
  g_print("System Monitoring Tool v0.1.0\n");
  g_print ("This application displays the system percent usage of the top 5 appliations currently running\n");
  g_print ("Usage: ./hello\n");
  g_print ("This application requires no arguments. It gets all needed data directly from the system.\n");
}

// Simulated usage update function
void update_usage(GtkWidget *label1, GtkWidget *label2, GtkWidget *label3, GtkWidget *label4, GtkWidget *label5) {
    char buffer[20];
    int usage;

    if (label1) {
        usage = rand() % 101;
        snprintf(buffer, sizeof(buffer), "%d%%", usage);
        gtk_label_set_text(GTK_LABEL(label1), buffer);
    }
    if (label2) {
        usage = rand() % 101;
        snprintf(buffer, sizeof(buffer), "%d%%", usage);
        gtk_label_set_text(GTK_LABEL(label2), buffer);
    }
    if (label3) {
        usage = rand() % 101;
        snprintf(buffer, sizeof(buffer), "%d%%", usage);
        gtk_label_set_text(GTK_LABEL(label3), buffer);
    }
    if (label4) {
        usage = rand() % 101;
        snprintf(buffer, sizeof(buffer), "%d%%", usage);
        gtk_label_set_text(GTK_LABEL(label4), buffer);
    }
    if (label5) {
        usage = rand() % 101;
        snprintf(buffer, sizeof(buffer), "%d%%", usage);
        gtk_label_set_text(GTK_LABEL(label5), buffer);
    }
}

gboolean on_update_data(gpointer data) {
    GtkWidget **labels = (GtkWidget **)data;

    // Ensure labels array is valid
    if (!labels) return FALSE;

    // Update the usage labels
    update_usage(labels[0], labels[1], labels[2], labels[3], labels[4]);

    return TRUE;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window, *grid;
    GtkWidget *App1, *App2, *App3, *App4, *App5, *Total;
    GtkWidget *CPU_App1, *CPU_App2, *CPU_App3, *CPU_App4, *CPU_App5;
    GtkWidget *CPU, *GPU, *RAM, *Battery, *Spacer;
    GtkWidget *button;


    // Create the main application window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "System Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    // Create a grid layout
    grid = gtk_grid_new();

    // Create labels
    App1 = gtk_label_new("\nApp 1\t");
    App2 = gtk_label_new("\nApp 2\t");
    App3 = gtk_label_new("\nApp 3\t");
    App4 = gtk_label_new("\nApp 4\t");
    App5 = gtk_label_new("\nApp 5\t");
    Total = gtk_label_new("\nTotal Usage\t");
    CPU = gtk_label_new("\nCPU Usage\t");
    GPU = gtk_label_new("\nRam Usage\t");
    RAM = gtk_label_new("\nRam Usage\t");
    Battery = gtk_label_new("\nBattery Usage\t");
    Spacer = gtk_label_new("\n\t");

    CPU_App1 = gtk_label_new("0%");
    CPU_App2 = gtk_label_new("0%");
    CPU_App3 = gtk_label_new("0%");
    CPU_App4 = gtk_label_new("0%");
    CPU_App5 = gtk_label_new("0%");

    //Creates and labels the help button
    button = gtk_button_new_with_label("Help");
    g_signal_connect (button,"clicked",G_CALLBACK(print_help), NULL);


    // Attach widgets to the grid
    gtk_grid_attach(GTK_GRID(grid), App1, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App1, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App2, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App2, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App3, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App3, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App4, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App4, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), App5, 1, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), CPU_App5, 2, 5, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), CPU, 2,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), GPU, 3,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), RAM, 4,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), Battery, 5,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), Spacer, 6,0,1,1);

    gtk_grid_attach(GTK_GRID(grid), button, 0,0,1,1);

    // Set the grid as the child of the window
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Create labels array on the heap
    GtkWidget **labels = g_new(GtkWidget *, 5);
    labels[0] = CPU_App1;
    labels[1] = CPU_App2;
    labels[2] = CPU_App3;
    labels[3] = CPU_App4;
    labels[4] = CPU_App5;

    // Schedule periodic updates
    g_timeout_add(1000, on_update_data, labels);

    // Show the window
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_REPLACE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

