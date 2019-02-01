#ifndef MC2_H_
#define MC2_H_
int num_coms = 3;
char coms[32][128];
char coms_des[32][75];
char coms_line[32][100];
char coms_option[32][8];
char coms_args[32][128];
int coms_back[32];

int current_back = 0;
int num_back = 0;
int back_comp = 0;
pid_t back_pids[32];
char back_args[32][128];
int back_active[32];
int back_pipes[32][2];
int back_num[32];

int num_sets = 5;
char sets[5][128];
char sets_des[5][75];
char sets_line[5][100];
char sets_option[5][8];

void initalize();
void printPrompt();
void runLSCom();
void runCom(char *option);
void printStatistics(struct rusage usage);
int checkValidOption(char *option);
void addCommand();
void checkBackground();

void changeDirectory();
void printDirectory();
void printProcesses();

int getBackIndex(pid_t pid);
char* trim();
char *const* getArguments(char *arguments, char *command);
char** copyArray(char array[][128], int i);
char* trimArray(char args[][128], int i, int n);

void attemptExit();
#endif