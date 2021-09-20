#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

bool initInteractive(){
	
	char *tashArgs[3];
	char *inputLine;
	
	size_t length = 0;
	ssize_t line_length;
	
	tashArgs[1] = NULL;
	printf("tash> ");
	
	while ( (line_length = getline(&inputLine, &length, stdin) != -1 ) ) {
		
		printf("Command retrieved: %s and line_length is %zu", inputLine, line_length);
		
		tashArgs[0] = strtok(inputLine,"\n");
		
		if ( strcmp(tashArgs[0], "exit") ) {
			// call exit syscall with 0 as param
			exit(0);
			return true;
		}
		
		else if ( strcmp(tashArgs[0], "cd") ) {
			// call cd function using chdir() with arg as a syscall. if 0 or >1 arg, throw an error. if chdir() fails, throw an error
			return true;
		}
		
		else if ( strcmp(tashArgs[0], "path" ) ) {
			// call path function with 0 or more args with each arg space-separated. 
			// by default, the shell should execute only the built-in cmds viz. exit, cd, path
			// if path is called with args, it should overwrite the previous path and 
			// only execute the files present in the directories specified by the args
			
			//check for redirection
			return true;
		}
		
		else if ( strtok(inputLine, "&") ) {
			// parallel cmd processing
			// check for redirection 
			return true;
		}
		
		else if ( strtok(inputLine, ">") ) {
			// redirection of the cmd's o/p to a file
			return true;
		}
		
		
		
		else {
			// normal execution flow of code by reading the cmd and its args
			// take care of multiple white-spaces in the line given as i/p
			return true;
			
		}
		
	}
	
	return false;
}
