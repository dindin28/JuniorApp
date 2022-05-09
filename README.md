# Git
To download the latest source from the Git server do this:

    git clone https://github.com/dindin28/JuniorApp.git

# Building app
To compile program do next in JuniorApp folder:

    mkdir build
    cd build
    cmake ..
    make

# Usage

**JuniorApp [any integer]** 

Any integer is optional argument  
If not set randoming timer from 0.001 to 1 second

## Example
    ./JuniorApp - with random mechanism
    ./JuniorApp 1 - delay 1 second before increasing

# How to test (Example)
Starting application with 2 seconds delay

    ./JuniorApp 2

![first_terminal_image](https://github.com/dindin28/JuniorApp/blob/bfa8903ff0ef9cdd8d8b34e757a4168100845ec6/readme_files/first_terminal.jpg)

In the second terminal use

    kill -SIGTERM <PID>

![second_terminal_image](https://github.com/dindin28/JuniorApp/blob/bfa8903ff0ef9cdd8d8b34e757a4168100845ec6/readme_files/second_terminal.jpg)