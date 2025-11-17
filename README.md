# System Programming Lab 11 Multiprocessing

Name: Ezan Hamdia
Section: 131

1. Implementation Overview

My program, mandelmovie, generates a 50-frame animation of the Mandelbrot set by using multiprocessing to speed up the rendering.

The program is written in C and managed with Git. The core logic is as follows:

It uses the getopt() function to parse a command-line argument -n, which specifies the maximum number of child processes to run simultaneously. If no argument is provided, it defaults to 1.

It runs a for loop from 0 to 49 (for the 50 frames). Inside the loop, it calculates the correct scale for the current frame to create a "zoom-in" effect.

To manage the process pool, it uses a counter, active_children. If the number of active children is already at the user-specified limit, the parent process calls wait(NULL) to pause and wait for any child to finish, freeing up a slot.

Once a slot is free, the parent uses fork() to create a new child.

The child process is responsible for rendering one frame. It converts the required parameters (x-coordinate, y-coordinate, and the new scale) into strings. It then uses execlp() to replace its own code with the provided ./mandel program, passing in all the calculated arguments.

The parent process increments the active_children counter after forking and immediately continues its loop to launch the next frame.

After the main loop has launched all 50 jobs, a final while loop calls wait(NULL) repeatedly until all remaining children have finished, ensuring the program only exits after all frames are generated.

2. Runtime Results

Here is a graph plotting the total runtime (in seconds) against the number of processes used to generate the 50 frames.

![Mandelmovie Run Time](Mandelmovie-Run-Time.png)

3. Discussion

The graph shows adding more processes to a program doesn't always speed up the run time. Although there was a signifcant difference going from 1 to 5 processes, we didn't really see that same change going from 10 to 20. This is probably because of the systems limited number of cores. Once all cores are in use, adding more processes just gives the system more overhead to handle all the different processes, rather than adding more speed. 