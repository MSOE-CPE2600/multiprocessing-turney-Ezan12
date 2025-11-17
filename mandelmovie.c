//  Ezan Hamdia, Section 131
//  mandelmovie.c

#include <stdio.h>     
#include <stdlib.h>    
#include <unistd.h>    
#include <sys/wait.h>  

#define DEFAULT_PROCS 1
#define TOTAL_FRAMES 50

int main(int argc, char *argv[]) {
    int num_processes = DEFAULT_PROCS;
    int opt;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        
        if (opt == 'n') {
            num_processes = atoi(optarg); 
        } else if (opt == '?') { 
            
            fprintf(stderr, "Usage: %s -n <num_processes>\n", argv[0]);
            exit(EXIT_FAILURE); 
        } else {
           
            fprintf(stderr, "Usage: %s -n <num_processes>\n", argv[0]);
            exit(EXIT_FAILURE); 
        }
    }

    if (num_processes <= 0) {
        num_processes = DEFAULT_PROCS;
    }

    printf("Starting to generate %d frames using %d processes...\n", TOTAL_FRAMES, num_processes);

    double x_coord = -0.576504825;
    double y_coord =  0.657298208;
    double start_scale = 0.01;
    double end_scale = 0.0001;

    int active_children = 0; 
    pid_t pid;

    for (int i = 0; i < TOTAL_FRAMES; i++) {

        if (active_children == num_processes) {
            wait(NULL); 
            active_children--;
        }

        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);

        } else if (pid == 0) {
            double scale = start_scale + (end_scale - start_scale) * (double)i / (TOTAL_FRAMES - 1);
            
            char filename[100];
            sprintf(filename, "mandel%d.jpg", i);

            char x_str[50], y_str[50], s_str[50];
            sprintf(x_str, "%f", x_coord);
            sprintf(y_str, "%f", y_coord);
            sprintf(s_str, "%f", scale);
            
            printf("Child %d (Frame %d): Launching ./mandel -s %s -o %s\n",
                   getpid(), i, s_str, filename);

            execlp("./mandel",     
                   "mandel",       
                   "-x", x_str,    
                   "-y", y_str,    
                   "-s", s_str,    
                   "-o", filename, 
                   NULL);          

    
            perror("execlp");
            exit(1); 

        } else {
            active_children++; 
        }
    }
    
    printf("All 50 jobs launched. Waiting for remaining %d children...\n", active_children);
    while (active_children > 0) {
        wait(NULL);
        active_children--;
    }

    printf("All frames generated.\n");
    
    return 0;
}