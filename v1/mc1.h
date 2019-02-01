#ifndef MC1_H_
#define MC1_H_
int num_coms = 3;
char coms[32][128];
char coms_des[32][75];
char coms_line[32][100];
char coms_option[32][8];
char coms_args[32][128];

int num_sets = 4;
char sets[4][128];
char sets_des[4][75];
char sets_line[4][100];
char sets_option[4][8];

struct timeval tv;

void initalize();
void printPrompt();
void runLSCom();
void runCom(char *option);
void printStatistics(struct rusage usage);
int checkValidOption(char *option);
void addCommand();

void changeDirectory();
void printDirectory();

void startTimer();
double endTimer();
char* trim();
char *const* getArguments(char *arguments, char *command);
char** copyArray(char array[][128], int i);
char* trimArray(char args[][128], int i, int n);
#endif