#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
	
	char *myargs[3];
	char *line;
	size_t len = 0;
    	ssize_t line_len;

	myargs[1] = NULL;
    	printf("tash> ");
	while ((line_len = getline(&line, &len, stdin)) != -1) {
          printf("Retrieved line of length %zu :\n", line_len);
          printf("Command retrieved: %s", line);

		myargs[0] = strtok(line, "\n");
		int rc = fork();

		if(strcmp(myargs[0],"exit")){
			exit(0);
		}
		
		else if (strstr(line,"cd")){
			printf("cd command entered");
		}
		
		if(rc == 0) {
			printf("Hello I am child process with pid: %d\n",(int) getpid());
			
			execvp(myargs[0],myargs); 
		}

		else {
			int wc = wait(NULL);
			printf("wc = %d",wc);
			printf("tash> ");
		}
	}
	return 0;
}
