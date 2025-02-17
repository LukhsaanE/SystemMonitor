# System Monitoring Tool
This app tracks CPU usage, RAM usage, and the battery life remaining on your computer. It refreshes every second.

## Features
- **CPU Usage**: Tracks the usage of the apps that consume the most CPU (top 5) and the total CPU being used (given in %).
- **RAM**: Displays the total RAM being used by the top 5 app from the CPU and the total available (given in GB).
- **Battery Life**: Shows the remaining battery life.

## How to use
- Note: Only works for Windows
- Download the files needed in the Installation Needed.
- Download the repo.
- Open the command prompt and run the commmand `make` and `./readSystem --help`.
- Run `make usage_display` then run the command `./usage_display`.
  
### Example usage
https://github.com/user-attachments/assets/98687040-ccc8-4075-9c51-9b6573774916

## Installations Needed
The following needs to be downloaded be able to use the app. 
- [GTK Installation](https://www.gtk.org/docs/installations/windows#using-gtk-from-msys2-packages)
- [GCC with MinGW](https://code.visualstudio.com/docs/cpp/config-mingw)


