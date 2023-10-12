#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <pthread.h> 

void print_process_info(const char *pid) {
    char path[100];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);
    FILE *file = fopen(path, "r");

    if (file != NULL) {
        char line[256];
        char *token;
        int uid;
        char process_name[256];
        char state[256];

        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "Uid:", 4) == 0) {
                token = strtok(line, "\t ");
                token = strtok(NULL, "\t "); // Skip the "Uid:" label
                uid = atoi(token);
            } else if (strncmp(line, "Name:", 5) == 0) {
                token = strtok(line, "\t ");
                token = strtok(NULL, "\t "); // Skip the "Name:" label
                strncpy(process_name, token, sizeof(process_name));
                process_name[strlen(process_name)-1] = '\0';
            } else if (strncmp(line, "State:", 6) == 0) {
                token = strtok(line, "\t ");
                token = strtok(NULL, "\t "); // Skip the "State:" label
                strncpy(state, token, sizeof(state));
            }
        }
        fclose(file);

        struct passwd *pw = getpwuid(uid);
        if (pw != NULL) {
            printf("%-6s| %-15s| %-27s| %-7s|\n", pid, pw->pw_name, process_name, state);
        }
    }
}

void *list_process(void *targs)
{
        while (1) {
        printf("PID   | User           | PROCNAME                   | Estado |\n");
        printf("------+----------------+----------------------------+--------|\n");

        DIR *proc_dir = opendir("/proc");
        struct dirent *entry;

        if (proc_dir == NULL) {
            perror("Error opening /proc directory");
            return NULL;
        }
        int i=0;
        while ((entry = readdir(proc_dir)) != NULL && i<20) {
            // Check if the entry is a directory and represents a process ID
            if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
                print_process_info(entry->d_name);
                i++;
            }
        }

        closedir(proc_dir);

        // Wait for one second before updating the table
        sleep(1);
}
}

void *send_signal(void *targs) {
    while(1){
        int pid, signal;
        int read_numbers = scanf("%d %d", &pid, &signal);
        if(read_numbers==2){
        int result = kill(pid, signal);
            if (result == 0) {
                printf("Signal %d sent to process with PID %d.\n", signal, pid);
            } else {
                perror("Error sending signal");
            }
        }

        // Clear the input buffer
        while ((getchar()) != '\n');
}
}

int main()
{
    pthread_t thread_signal, thread_process;
    pthread_create(&thread_signal, NULL, list_process, NULL);
    pthread_create(&thread_process, NULL, send_signal, NULL);
    pthread_join(thread_signal, NULL);
    pthread_join(thread_process, NULL);  
}