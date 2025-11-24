//  Ezan Hamdia, Section 131
//  mandelmovie.c

#include <stdio.h>     
#include <stdlib.h>    
#include <unistd.h>    
#include <sys/wait.h>  

#define DEFAULT_PROCS 1
#define DEFAULT_THREADS 1  // New constant!
#define TOTAL_FRAMES 50

int main(int argc, char *argv[]) {
    int num_processes = DEFAULT_PROCS;
    int num_threads = DEFAULT_THREADS; // New variable!
    int opt;

    // Notice the updated argument string: "n:t:"
    // This tells getopt to look for -n (with a value) AND -t (with a value).
    while ((opt = getopt(argc, argv, "n:t:")) != -1) {
        
        if (opt == 'n') {
            num_processes = atoi(optarg);
        } else if (opt == 't') { // New logic for the -t flag
            num_threads = atoi(optarg);
        } else if (opt == '?') { 
            fprintf(stderr, "Usage: %s -n <num_processes> [-t <num_threads>]\n", argv[0]);
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Usage: %s -n <num_processes> [-t <num_threads>]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    // ... (rest of your startup checks)

    // ... your process pool logic will now start here ...
    printf("Starting to generate %d frames using %d processes...\n", TOTAL_FRAMES, num_processes);

    double x_coord = -0.743643;
    double y_coord = 0.131825;
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

            char x_str[50], y_str[50], s_str[50], t_str[10];;
            sprintf(x_str, "%f", x_coord);
            sprintf(y_str, "%f", y_coord);
            sprintf(s_str, "%f", scale);
            sprintf(t_str, "%d", num_threads);
            
            printf("Child %d (Frame %d): Launching ./mandel -s %s -o %s\n",
                   getpid(), i, s_str, filename);

            execlp("./mandel",     
                   "mandel",       
                   "-x", x_str,    
                   "-y", y_str,    
                   "-s", s_str,   
                   "-t", t_str, 
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