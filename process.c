#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include"my_shell.h"

int process(char **arglist){
	int rv = 0;
	if(arglist[0] == NULL)
		return rv;
	if(is_control_command(arglist[0]))
		rv = do_control_command(arglist);
	else if(ok_to_execute()){
		if(is_built_in_command(arglist[0]))
			do_built_in_command(arglist);
		else
			rv = execute(arglist);
	}
	return rv;	
}

int execute(char **arglist){
	extern char**environ;
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
		environ = var_to_environ();
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
