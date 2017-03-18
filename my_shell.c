#include"my_shell.h"
#include<string.h>
#define PROMPT ">"


int main(){     
	char *cmdline,*prompt,**arglist;
	int result;
	void setup();
	prompt = PROMPT;
	setup();
	while((cmdline = next_cmd(prompt,stdin)) != NULL){
		if((arglist = splitline(cmdline)) != NULL){
			result = process(arglist);
		}
		free(cmdline);
		cmdline = NULL;
	}


}
void setup(){
	extern char** environ;
	signal(SIGINT,SIG_IGN);
	var_init_from_environ(environ);
}
