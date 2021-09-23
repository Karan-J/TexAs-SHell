#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>

void exitSysCall();
void cdSysCall(char* cdArgs);
void pathSysCall(char* pathArgs);


void initInteractive(){
	
	char *tashArgs[3];
	char *inputLine;
	
	size_t length = 0;
	ssize_t line_length;
	
	tashArgs[1] = NULL;
	printf("tash> ");
	
	while ( (line_length = getline(&inputLine, &length, stdin) != -1 ) ) {


		if (inputLine == NULL) {
			return false;
		}		


//		printf("retrievedLineLength is %zu\n", line_length);
//		printf("Command retrieved: %s\n", inputLine);

		printf("Command retrieved: %s and line_length is %zu\n", inputLine, line_length);
		
		tashArgs[0] = strtok(inputLine,"\n");
		int pid = fork();
		
		if ( strcmp(tashArgs[0], "exit") == 0 ) {
			// call exit syscall with 0 as param
			printf("exit called\n");
			exitSysCall();
		}
		

		else if ( strcmp(tashArgs[0], "cd") == 0 ) {
			// call cd function using chdir() with arg as a syscall. if 0 or >1 arg, throw an error. if chdir() fails, throw an error
			printf("cd called\n");

			cdSysCall(*tashArgs);

		}
		
		else if ( strcmp(tashArgs[0], "path") == 0 ) {
			// call path function with 0 or more args with each arg space-separated. 
			// by default, the shell should execute only the built-in cmds viz. exit, cd, path
			// if path is called with args, it should overwrite the previous path and 
			// only execute the files present in the directories specified by the args
			
			//check for redirection
			printf("path called\n");

			pathSysCall(*tashArgs);

		}
		
		else if ( strtok(inputLine, "&") == 0 ) {
			// parallel cmd processing
			// check for redirection 
			// this is not working properly
			printf("& found\n");
			// call the input-parser and execute cmds
		}
		
		else if ( strtok(inputLine, ">") == 0 ) {
			// redirection of the cmd's o/p to a file
			// this is not working properly
			printf("> found\n");
			// redirect the o/p to some place
		}		
		
		else {
			// normal execution flow of code by reading the cmd and its args
			// take care of multiple white-spaces in the line given as i/p
//			printf("normal execution\n");
			if (pid == 0) {
				printf("Hi, I am a child process with pid %d\n", (int) getpid());
				execvp(tashArgs[0],tashArgs);
			}
			else {
				int waitCount = wait(NULL);
				if (waitCount == -1) {
					// there was an error in the child process
				}
				else {
					printf("Hi, I am parent and child with pid = %d completed execution\n",waitCount);
				}
				printf("tash> ");
			}
		}
	}
	printf("reached here\n");
}

int main() {
	initInteractive();
	return 0;
}
