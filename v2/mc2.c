#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/resource.h"
#include "sys/time.h"
#include "sys/wait.h"
#include "unistd.h"

#include "mc2.h"

/**
 * Main method of the program. Will loop indefinitly untill manually ended.
 * @param argc The number of arguments
 * @param argv The arguments as strings
 */
int main(int argc, char **argv) {
    initalize();

    printf("%s\n", "===== Mid-Day Commander, v2 =====");
    while(1) {
        printPrompt();
        checkBackground();
        char *option;
        unsigned int option_len = 8;
        option = malloc(option_len + 1);
        getline(&option, &option_len, stdin);

        trim(option);

        printf("%s\n", "");
        if(!checkValidOption(option)) {
            printf("%s\n\n", "Invalid Option");
            continue;
        }
        
        if(strcmp(option, "2") == 0) {
            runLSCom();
        } else if(strcmp(option, "a") == 0) {
            addCommand();
        } else if(strcmp(option, "c") == 0) {
            changeDirectory();
        } else if(strcmp(option, "e") == 0) {
            attemptExit();
        } else if(strcmp(option, "") == 0) {
            attemptExit();
        } else if(strcmp(option, "p") == 0) {
            printDirectory();
        } else if(strcmp(option, "r") == 0) {
            printProcesses();
        } else {
            runCom(option);
        }
    }
    return 0;
}

/**
 * Adds a command to the lsit
 */
void addCommand() {
    printf("%s\n", sets_line[0]);
    printf("%s", "Command to Add?:");
    char *input;
    char *arguments;
    unsigned int arg_len = 132;
    arguments = malloc(arg_len + 1);
    input = malloc(arg_len + 1);
    getline(&input, &arg_len, stdin);
    if(strlen(input) > 128) {
        printf("%s\n\n", "Error: Line must be less than 128 characters.");
        return;
    }

    trim(input);
    strcat(arguments, input);

    char args1[32][128] = {""};
    char *arg;
    int i = 1;
    arg = strtok(arguments, " ");
    while(arg != NULL) {
        if(i >= 32) {
            printf("%s\n\n", "Error: Over 32 arguments.");
            return;
        }
        strcpy(args1[i], arg);
        i++;
        arg = strtok(NULL, " ");
    }
    i--;

    coms_back[num_coms] = 0;
    if(strcmp(args1[i], "&") == 0) {
        coms_back[num_coms] = 1;
        i--;
    }

    strcpy(coms[num_coms], args1[1]);

    strcpy(coms_des[num_coms], "User added Command");

    strcat(coms_line[num_coms], "-- Command: ");
    strcat(coms_line[num_coms], trimArray(args1, i, 1));
    strcat(coms_line[num_coms], " --");
    
    char *option;
    option = malloc(8 + 1);
    sprintf(option, "%d", num_coms);
    strcat(coms_option[num_coms], option);

    strcat(coms_args[num_coms], trimArray(args1, i, 1));

    printf("%s %s\n\n", "Okay, added with ID", option);

    /*printf("%s\n", coms[num_coms]);
    printf("%s\n", coms_des[num_coms]);
    printf("%s\n", coms_line[num_coms]);
    printf("%s\n", coms_option[num_coms]);
    printf("%s\n", coms_args[num_coms]);*/

    num_coms++;
}

/** 
 * Speacil handling of the LS command
 */
void runLSCom() {
    struct rusage usage;
    int status;
    int options = 0;

    printf("%s\n", coms_line[2]);
    printf("%s", "Arguments?:");
    char *arguments;
    unsigned int arg_len = 128;
    arguments = malloc(arg_len + 1);
    getline(&arguments, &arg_len, stdin);
    
    printf("%s", "Path?:");
    char *path;
    unsigned int path_len = 128;
    path = malloc(path_len + 1);
    getline(&path, &path_len, stdin);

    trim(arguments);
    trim(path);

    strcat(arguments, " ");
    strcat(arguments, path);
    char *const *args = getArguments(arguments, "/bin/ls");

    printf("%s\n", "");

    pid_t pid = fork();
    // If Child
    if(pid == 0) {
        execvp(args[0], args);
        
        printf("%s\n", "Should not appear");
    // If parent
    } else {
        int result = wait4(pid, &status, options, &usage);
        printf("%s\n", "");
        if(result == -1) {
            printf("Error: %s\n", strerror(errno));
        }
        printStatistics(usage);
    }
}

/**
 * Runs a generic command
 * @param option The character to denote the command
 */
void runCom(char *option) {
    int i = atoi(option);
    struct rusage usage;
    int status;
    int options = 0;

    printf("%s\n", coms_line[i]);

    char command[128];
    strcpy(command, "/bin/bash");

    char *argument;
    argument = malloc(128 + 1);
    strcat(argument, "-c ");
    strcat(argument, coms_args[i]);
    char *const* args = getArguments(argument, command);

    // Background
    if(coms_back[i] && pipe(back_pipes[current_back]) < 0) {
            printf("%s\n", "Error setting up pipe.");
            return;
    }

    pid_t pid = fork();
    // If Child
    if(pid == 0) {
        if(coms_back[i]) {
            dup2(back_pipes[current_back][1], STDOUT_FILENO);
            close(back_pipes[current_back][1]);
            close(back_pipes[current_back][0]);
        }
        execvp(args[0], args);
        //execl(command, command, "-c", coms[i], (char*) NULL);
        printf("%s\n", "Should not appear");
    // If parent - background task
    } else if(coms_back[i]) {
        close(back_pipes[current_back][1]);

        back_pids[current_back] = pid;
        strcpy(back_args[current_back], coms_args[i]);
        back_active[current_back] = 1;
        back_num[current_back] = ++num_back;

        printf("[%d] %d\n\n", back_num[current_back] - back_comp, pid);

        current_back++;
        if(current_back >= 32) {
            current_back = 0;
        }
    // If parent
    } else {
        int result = wait4(pid, &status, options, &usage);
        printf("%s\n", "");
        if(result == -1) {
            printf("Error: %s\n", strerror(errno));
        }
        printStatistics(usage);
    }
}

/**
 * Checks if any child process have ended
 */
void checkBackground() {
    int status;
    int flag = 0;
    pid_t pid = 1;
    while(pid != -1 && pid != 0) {
        struct rusage usage;
        pid = wait3(&status, WNOHANG, &usage);
        if(pid != -1 && pid != 0) {
            flag = 1;

            int i = getBackIndex(pid);
            printf("\n\n%s [%d] --\n", "-- Job Complete", back_num[i] - back_comp);
            printf("%s %d\n", "Process ID:", pid);

            back_comp++;
            current_back--;
            back_active[i] = 0;

            char* buffer;
            buffer = malloc(4096 + 1);
            ssize_t bytes = 0;
            while(bytes <= 0) {
                bytes = read(back_pipes[i][0], buffer, 4096);
                if(bytes > 0) {
                    printf("%s\n", buffer);
                }
            }
            printStatistics(usage);
        }
    }
    if(flag) {
        printPrompt();
    }
}

/**
 * Prints out the usage information of a command
 * @param usage The rusage object to read from
 */
void printStatistics(struct rusage usage) {
    printf("%s\n", "-- Statistics --");
    printf("Elasped TIme: %2.1f milliseconds\n", usage.ru_utime.tv_sec / 1000.0 + usage.ru_utime.tv_usec / 1000.0);
    printf("Page Faults: %ld\n", usage.ru_majflt);
    printf("Page Faults (reclaimed): %ld\n\n", usage.ru_minflt);
}

/**
 * Changes the working directory
 */
void changeDirectory() {
    printf("%s\n", sets_line[1]);
    printf("%s", "New Directory?:");

    char *dir;
    unsigned int dir_len = 128;
    dir = malloc(dir_len + 1);
    getline(&dir, &dir_len, stdin);

    trim(dir);

    int result = chdir(dir);    
    if(result == -1) {
        printf("Error: %s\n", strerror(errno));
    }
    printf("%s\n", "");
}

/**
 * Prints the current directory
 */
void printDirectory() {
    printf("%s\n", sets_line[3]);
    char *cwd;
    size_t cwd_len = 128;
    cwd = malloc(cwd_len +1);
    getcwd(cwd, cwd_len);
    printf("%s: %s\n\n", "Directory", cwd);
}

/**
 * Prints out the current background processes
 */
void printProcesses() {
    printf("%s\n", sets_line[4]);
    int i;
    for(i = 0; i < current_back; i++) {
        if(back_active[i]) {
            printf("[%d] %d : %s\n", back_num[i] - back_comp, back_pids[i], back_args[i]);
        }
    }
    printf("%s\n", "");
}

/**
 * Checks the given command to check if valid
 * @param option The option to check
 * @returns Whether or not the option is valid
 */
int checkValidOption(char *option) {
    int i;
    for(i = 0; i < num_coms; i++) {
        if(strcmp(coms_option[i], option) == 0) {
            return 1;
        }
    }
    for(i = 0; i < num_sets; i++) {
        if(strcmp(sets_option[i], option) == 0) {
            return 1;
        }
    }
    if(strcmp(option, "") == 0) {
        return 1;
    }
    return 0;
}

/**
 * Gets the background index of the pid
 * @param pid The pid to look for
 * @returns The index of the pid, -1 if not found
 */
int getBackIndex(pid_t pid) {
    int i;
    for(i = 0; i < 32; i++) {
        if(back_pids[i] == pid) {
            return i;
        }
    }
    return -1;
}

/**
 * Initalizes the starting commands
 */
void initalize() {
    strcpy(coms[0], "whoami");
    strcpy(coms[1], "last");
    strcpy(coms[2], "ls");

    strcpy(coms_des[0], "Prints out the result of the whomai command");
    strcpy(coms_des[1], "Prints out the result of the last command");
    strcpy(coms_des[2], "Prints out the results of a listing of a user-specificed path");

    strcpy(coms_line[0], "-- Who Am I? --");
    strcpy(coms_line[1], "-- Last Logins --");
    strcpy(coms_line[2], "-- Directory Listing --");

    strcpy(coms_option[0], "0");
    strcpy(coms_option[1], "1");
    strcpy(coms_option[2], "2");

    strcpy(coms_args[0], "whoami");
    strcpy(coms_args[1], "last");
    strcpy(coms_args[2], "ls");

    coms_back[0] = 0;
    coms_back[1] = 0;
    coms_back[2] = 0;

    strcpy(sets[0], "add command");
    strcpy(sets[1], "change directory");
    strcpy(sets[2], "exit");
    strcpy(sets[3], "pwd");
    strcpy(sets[4], "running processes");

    strcpy(sets_des[0], "Adds a new command to the menu.");
    strcpy(sets_des[1], "Changes process working directory");
    strcpy(sets_des[2], "leave Mid-Day Commander");
    strcpy(sets_des[3], "Prints working directory");
    strcpy(sets_des[4], "Print list of running processes");

    strcpy(sets_line[0], "-- Add a Command --");
    strcpy(sets_line[1], "-- Change Directory --");
    strcpy(sets_line[2], "");
    strcpy(sets_line[3], "-- Current Directory --");
    strcpy(sets_line[4], "-- Background Processes --");

    strcpy(sets_option[0], "a");
    strcpy(sets_option[1], "c");
    strcpy(sets_option[2], "e");
    strcpy(sets_option[3], "p");
    strcpy(sets_option[4], "r");
}

/**
 * Prints out the prompt for a command
 */ 
void printPrompt() {
    printf("%s\n", "G'day, Commander! What command would you like to run?");
    int i;
    for(i = 0; i < num_coms; i++) {
        printf("   %s. %-8s : %s\n", coms_option[i], coms_args[i], coms_des[i]);
    }
    for(i = 0; i < num_sets; i++) {
        printf("   %s. %-18s : %s\n", sets_option[i], sets[i], sets_des[i]);
    }
    printf("%s", "Option?:");
}

/**
 * Removes the final new line character from the end of a string
 * @param string The strig to trim
 * @returns The modified string
 */
char* trim(char *string) {
    int str_len = strlen(string);
    char *end = string + str_len - 1;
    end = end;
    end[0] = '\0';
    return string;
}

/**
 * Splits a given string of arguments and a command into a array of args
 * @param arguments A string of the arugments seperated by spaces
 * @param command The command to run
 * @returns A const chara array of the arguments
 */
char *const*  getArguments(char *arguments, char *command) {
    char args1[32][128] = {""};
    char *arg;
    int i = 1;
    arg = strtok(arguments, " ");
    while(arg != NULL) {
        strcpy(args1[i], arg);
        i++;
        arg = strtok(NULL, " ");
    }
    i--;
    strcpy(args1[0], command);

    char *const* copy = copyArray(args1, i);
    /*int j;
    for(j = 0; j <= i; j++) {
        printf("%d %d %s\n", j, i, copy[j]);
    }*/
    return copy;
}

/**
 * Copies an array up to the given size
 * @param array The array to copy
 * @param i The number of elements to copy
 * @returns The const char array
 */
char** copyArray(char array[][128], int i) {
    int j;
    char **copy = (char**) malloc((i + 1) * 128);

    for(j = 0; j <= i; j++) {
        copy[j] = (char*) malloc(128 + 1);
        strcpy(copy[j], array[j]);
    }
    return copy;
}

/**
 * Returns the arguments minus the first n elements
 * @param args The array list
 * @param i The total number of elements
 * @param n The number of elements to skip
 * @return A string of the last elements
 */
char* trimArray(char args[][128], int i, int n) {
    int j;
    char *line;
    line = malloc(128 + 1);
    if(n > i) {
        return "";
    }
    strcat(line, args[n]);
    for(j = n + 1; j <= i; j++) {
        strcat(line, " ");
        strcat(line, args[j]);
    }
    return line;
}

/**
 * Will wait for any background tasks and then exit
 */
void attemptExit() {
    if(num_back != back_comp) {
        printProcesses();
    }
    while(num_back != back_comp) {
        checkBackground();
    }
    printf("%s\n", "Logging you out, Commader.");
    exit(0);
}