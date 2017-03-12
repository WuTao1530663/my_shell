#include<sys/wait.h>
#include<signal.h>
#include<string.h>
#include"my_shell.h"

enum states {NEUTRAL,WANT_THEN,THEN_BLOCK,ELSE_BLOCK};
enum results {SUCCESS,FAIL};
static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int ok_to_execute(){
	/*
	purpose:determine the shell shold execute a cmd
	returns:1 for yes,0 for no
	notes: if in THEN_BLOCK and if_result is SUCCESS return yes
		if in THEN_BLOCK and if_result is FAIL return no
		if in ELSE_BLOCK and if_result is FAIL return yes
		if in ELSE_BLOCK and if_result is SUCCESS return no
		if in WANT_THEN then error
	*/
	int rv = 1;
	if(if_state == WANT_THEN){
		perror("if_state error");
		rv = 0;
	}
	else if(if_state == THEN_BLOCK && if_result == FAIL)
		rv = 0;
	else if(if_state == ELSE_BLOCK && if_result == SUCCESS)
		rv = 0;
	return rv;
}

int is_control_command(char *cmd){
	return (strcmp(cmd,"if")==0||strcmp(cmd,"then")==0||strcmp(cmd,"fi")==0||strcmp(cmd,"else")==0);
}

int do_control_command(char **args){
	/*purpose: Process control command--change if_state or detect error
	 *returns:0 if ok,-1 for systax error
	*/
	char *cmd = args[0];
	int rv = -1;
	if(strcmp(cmd,"if")==0){
		if(if_state != NEUTRAL)
			perror("if_state error");
		else{
			last_stat = process(args + 1);//execute cmd after if
			if_result = (last_stat == 0 ? SUCCESS:FAIL);
			if_state = WANT_THEN;
			rv = 0;
	}
	}
	else if(strcmp(cmd,"then") == 0){
		if(if_state != WANT_THEN)
			perror("if_state error");
		if_state = THEN_BLOCK;
		if(ok_to_execute())
			process(args + 1);
		rv = 0;
	}
	else if(strcmp(cmd,"else") == 0){
		if(if_state != THEN_BLOCK)
			perror("if_state error");
		if_state = ELSE_BLOCK;
		if(ok_to_execute())
			process(args + 1);
		rv = 0;	
	}
	else if(strcmp(cmd,"fi") == 0){
		if(if_state!=ELSE_BLOCK)
			perror("if_state error");
		else{
			if_state = NEUTRAL;
			rv = 0;
		    }
		}
	return rv;


}

int process(char **arglist){
	int rv = 0;
	if(arglist[0] == NULL)
		return rv;
	if(is_control_command(arglist[0]))
		rv = do_control_command(arglist);
	else if(ok_to_execute())
		rv = execute(arglist);
	return rv;	
}

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
