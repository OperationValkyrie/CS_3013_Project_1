#ifndef MC0_H_
#define MC0_H_
int num_coms = 3;
char coms[3][20];
char coms_des[3][75];
char coms_line[3][100];
char coms_option[3][1];

struct timeval tv;

void initalize();
void printPrompt();
void runLSCom();
void runCom(char option);
void printStatistics(struct rusage usage);
int checkValidOption(char option);

void startTimer();
double endTimer();
char* trim();
#endif