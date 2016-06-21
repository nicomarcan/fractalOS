x64-Kernel configures some of the basic elements of the system (such as the keyboard, the RTC and display), and offers a minimal shell so as to test these features. This project is based on x64BareBones, a basic setup to develop operating systems for the Intel 64 bits architecture.

Environment setup:
1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc make

2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

  user@linux:$ cd Toolchain
  user@linux:$ make all

3- Build the Kernel

From the x64-Kernel project directory run:

  user@linux:$ make all

4- Run the kernel

From the x64-Kernel project directory run:

  user@linux:$ ./run.sh

5- Test the functionalities of the kernel. Type 'help' for a list of commands. Among other features, you will be able to change both font and background colour, play Fanorona ©, and display a fractal (Dragon Curve).

Programming instructions:

There is a small set of system calls available, which are mainly focused in interacting with the key peripherals of the system, such as the keyboard and the screen. Among these syscalls you will find the following:

0-  sys_read            Read from a file descriptor (Only keyboard implemented)
1-  sys_write           Write to a file descriptor  (Only screen available)
8-  sys_mem             Obtain memory from the system
9-  sys_sleep           Sleep n ticks (timer tick)
10  sys_pixel           Write a pixel
11- sys_clearscreen     Clear the screen
12- sys_time            Get the time from the RTC
13- sys_set_color       Set font color
14- sys_set_back_color  Set background color

All of these syscalls are available through int 80h. The syscall number is the value passed at rax. See syscalls.h for further details. 

x64-Kernel
Authors: Tomas Raies, M. Alejo Saques

x64BareBones
Author: Rodrigo Rearden (RowDaBoat)
Collaborator: Augusto Nizzo McIntosh
