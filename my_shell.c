#include<string.h>
#include<sys/wait.h>
#include<signal.h>
#include"my_shell.h"
#define PROMPT ">"


int main(){     
        char *cmdline,*prompt,**arglist;
        int result;
        void set_sig();
        prompt = PROMPT;
        set_sig();
        while((cmdline = next_cmd(prompt,stdin)) != NULL){
                if((arglist = splitline(cmdline)) != NULL){
                        result = execute(arglist);
//			freelist(arglist);
                }
		free(cmdline);
		cmdline = NULL;
        }        
}

void set_sig(){
	signal(SIGINT,SIG_IGN);
}
