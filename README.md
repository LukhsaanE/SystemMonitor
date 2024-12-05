# System Monitoring Tool
This app tracks CPU usage, RAM usage, and the battery life remaining on your computer. It refreshes every second.

## Features
- **CPU Usage**: Tracks the usage of the apps that consume the most CPU (top 5) and the total CPU being used (given in %).
- **RAM**: Displays the total RAM being used by the top 5 app from the CPU and the total available (given in GB).
- **Battery Life**: Shows the remaining battery life.

## How to use

### Example usage

## Installations Needed
The following needs to be downloaded be able to use the app. 
- [GTK Installation](https://www.gtk.org/docs/installations/windows#using-gtk-from-msys2-packages)
- [GCC with MinGW](https://code.visualstudio.com/docs/cpp/config-mingw)

## Testing the app
- The app could not be conventially tested with specific inputs and outputs, as the outpus are constantly changing.
- The way we decided to test it is matching up the specific stats with the task manager for Windows. 
- This way we could find out if the outputs were actually accurate. 

