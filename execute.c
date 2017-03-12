#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include"my_shell.h"

int execute(char **arglist){
	int pid;
	int child_info = -1;
	if (*arglist == NULL)
		return 0;
	if (strcmp("exit",*arglist) == 0) 
		exit(0);
	if((pid = fork()) == -1)
		perror("fork");
	else if(pid == 0){
		signal(SIGINT,SIG_DFL);
		execvp(arglist[0],arglist);
		perror("cannot execute command");
		exit(1);
}
	else{
		if(wait(&child_info) == -1)
			perror("wait");
		
	}
	return child_info;
}
