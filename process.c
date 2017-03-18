#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include"my_shell.h"

static int HAS_BG_SYMBOL = 0;
static int has_bg_symbol(char **arglist){
	for(int i = 0;arglist[i];++i)
		if(arglist[i + 1] == NULL && strcmp(arglist[i],"&") == 0)
			{
				arglist[i] = 0;
				return 1;
			}
	return 0;
}

int process(char **arglist){
	int rv = 0;
	if(arglist == NULL)
		return rv;
	redirect(arglist);
	if(is_control_command(*arglist) || is_in_while_block())
		rv = do_control_command(arglist);
	
	else if(ok_to_execute()){
		if(is_built_in_command(*arglist))
			do_built_in_command(arglist);
		else
			rv = execute(arglist);
	}
	recover_stdio();
	return rv;	
}


int execute(char **arglist){
	extern char**environ;
	int pid;
	int i,j;
	int child_info = -1;
	int pfd[MAXPIPES][2];
	int split_locs[MAXPIPES + 1];
	int pipe_num,cmd_num = 1;
	if (*arglist == NULL)
		return 0;
	if(has_bg_symbol(arglist))
		HAS_BG_SYMBOL = 1;
	split_locs[0] = -1;
	pipe_split_args(arglist,split_locs,&cmd_num);
	pipe_num = cmd_num - 1;
	pipe_create(pfd,pipe_num);
	for(i = 0;i < cmd_num;++i){
		if((pid = fork()) == -1)
			perror("fork");
		else if(pid == 0)
			break;
	}
	do_pipe(pfd,i,pipe_num,pid);
	arglist = arglist + split_locs[i] + 1;
	if(pid == 0){
		signal(SIGINT,SIG_DFL);
		environ = var_to_environ();
		execvp(arglist[0] ,arglist);
		perror("cannot execute command");
		exit(1);
	}
	else{
		if(HAS_BG_SYMBOL)
			printf("process %d running in the background\n",pid);
		for(i = 0;i < cmd_num;++i)//if HAS_BG_SYMBOL is true then set the WNOHANG flag
			waitpid(-1,&child_info,HAS_BG_SYMBOL?WNOHANG:0);
	}
	return child_info;
}

