#include"my_shell.h"
#include<string.h>
void pipe_split_args(char **arglist,int split_locs[],int *cmd_num_p){

	for(int i = 0,j = 0;arglist[i];++i){//record the locations of the symbol '|' and set the pointer 
		if(strcmp(arglist[i],"|") == 0){
			arglist[i] = 0;
			split_locs[++j] = i;
			(*cmd_num_p)++;
		}
	}
}

void pipe_create(int pfd[][2],int pipe_num){
	for(int i = 0;i < pipe_num;++i)
		pipe(pfd[i]);
}

void do_pipe(int pfd[][2],int pipe_index,int pipe_num,int pid){

	int j;
	if (pid != 0){
		for(int i = 0;i < pipe_num;i++){
			close(pfd[i][0]);
			close(pfd[i][1]);
		}
	}
	else{
		if(pipe_index == 0){
			dup2(pfd[0][1], STDOUT_FILENO);
			close(pfd[0][0]);
			close(pfd[0][1]);
			for (j=1; j<pipe_num; j++) {
				close(pfd[j][0]);
				close(pfd[j][1]);
			}
		}
		else if(pipe_index == pipe_num){
			dup2(pfd[pipe_num-1][0],STDIN_FILENO);
			close(pfd[pipe_num-1][1]);
			close(pfd[pipe_num - 1][0]);
			for(j = 0;j <pipe_num -1;j++){
				close(pfd[j][0]);
				close(pfd[j][1]);       
			}
		}
		else{
			dup2(pfd[pipe_index - 1][0],STDIN_FILENO);
			close(pfd[pipe_index - 1][1]);
			dup2(pfd[pipe_index][1],STDOUT_FILENO);
			close(pfd[pipe_index][0]);
			for(j = 0;j <pipe_num;j++){
				if(j != pipe_index || j!= pipe_index -1){
					close(pfd[j][0]);
					close(pfd[j][1]);
				}

			}

		}
	}
}	

