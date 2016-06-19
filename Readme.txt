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

x64-Kernel
Authors: Tomas Raies, M. Alejo Saques

x64BareBones
Author: Rodrigo Rearden (RowDaBoat)
Collaborator: Augusto Nizzo McIntosh
