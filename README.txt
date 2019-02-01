Jonathan Chang
CS 3011 C02
Project 1

This project builds an interactive program with uses fork
and execvp to create new processes and then execute commands.
In v0, the user can only execute the pre-programmed commands 
and much use CTRL-c to exit. In v1, the user can now input up
to 32 different commands each up to 128 character and 32 arguemnts.
In v2, the user can now run commands in the background by ending
the command with a &.

In v2, the background tasks are tracked in an pid_t array with 
other arrays to track the command, its number, and whether is is active.
This project can only support up to 32 background tasks at one time.
This program also assumes that the 1st background task has completed
before the 33rd is started as the arrays looped around. Whenever the program prints out the prompt, it checks if any of its childen
have finished and if so prints out the statisitcs and output.

This program was tested with the test.txt file inside of each of the folders
Inidividual features were tested manuelly with liberal use of printfs.

