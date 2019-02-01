#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/resource.h"
#include "sys/time.h"
#include "sys/wait.h"
#include "unistd.h"

#include "mc0.h"

/**
 * Main method of the program. Will loop indefinitly untill manually ended.
 * @param argc The number of arguments
 * @param argv The arguments as strings
 */
int main(int argc, char **argv) {
    initalize();

    printf("%s\n", "===== Mid-Day Commander, v0 =====");
    while(1) {
        printPrompt();
        char option = getchar();
        getchar();
        printf("%s\n", "");
        if(!checkValidOption(option)) {
            printf("%s\n\n", "Invalid Option");
            continue;
        }
        switch(option) {
            case '2':
                runLSCom();
                break;
            default:
                runCom(option);
                break;
        }
    }
    return 0;
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
    unsigned int arg_len = 20;
    arguments = malloc(arg_len + 1);
    getline(&arguments, &arg_len, stdin);
    //char *args;
    //args = strtok(arguments, " ");

    printf("%s", "Path?:");
    char *path;
    unsigned int path_len = 100;
    path = malloc(path_len + 1);
    getline(&path, &path_len, stdin);

    trim(arguments);
    trim(path);

    printf("%s\n", "");
    startTimer();

    pid_t pid = fork();
    // If Child
    if(pid == 0) {
        char command[30];
        strcpy(command, "/bin/ls");

        if(strcmp(arguments, "") == 0) {
            execl(command, command, path, (char*) NULL);
        } else {
            execl(command, command, path, arguments, (char*) NULL);
        }
        
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
void runCom(char option) {
    int i = atoi(&option);
    struct rusage usage;
    int status;
    int options = 0;

    printf("%s\n", coms_line[i]);
    startTimer();

    pid_t pid = fork();
    // If Child
    if(pid == 0) {
        char command[30];
        strcpy(command, "/bin/bash");
        execl(command, command, "-c", coms[i], (char*) NULL);
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
 * Prints out the usage information of a command
 * @param usage The rusage object to read from
 */
void printStatistics(struct rusage usage) {
    printf("%s\n", "-- Statistics --");
    printf("Elasped TIme: %2.1f milliseconds\n", endTimer());
    printf("Page Faults: %ld\n", usage.ru_majflt);
    printf("Page Faults (reclaimed): %ld\n\n", usage.ru_minflt);
}

/**
 * Checks the given command to check if valid
 * @param option The option to check
 * @returns Whether or not the option is valid
 */
int checkValidOption(char option) {
    int i;
    for(i = 0; i < num_coms; i++) {
        if(coms_option[i][0] == option) {
            return 1;
        }
    }
    return 0;
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
}

/**
 * Prints out the prompt for a command
 */ 
void printPrompt() {
    printf("%s\n", "G'day, Commander! What command would you like to run?");
    int i;
    for(i = 0; i < num_coms; i++) {
        printf("   %d. %-8s : %s\n", i, coms[i], coms_des[i]);
    }
    printf("%s", "Option?:");
}

/**
 * Starts the timer
 */
void startTimer() {
    gettimeofday(&tv, NULL);
}

/**
 * Ends the timer and retursn the elasped time
 * @returns The elasped time in milliseconds
 */
double endTimer() {
    double start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    gettimeofday(&tv, NULL);

    double end_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return end_time - start_time;
}

/**
 * 
 */
char* trim(char *string) {
    int str_len = strlen(string);
    char *end = string + str_len - 1;
    end = end;
    end[0] = '\0';
    return string;
}