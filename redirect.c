#include<string.h>
#include"my_shell.h"

/*
interface:
	void redirect(args) process the things about io redirect
	void recover_stdio() recover standard IO stream to default
private method:
	static void input_redirect(filename) :redirect stdin to the file named filename
	static void output_redirect(filename,mode):redirect stdout to the file named filename and if set the mode means append
	static void delete_io_symbol(args,loc): delete the redirect symbol likes '>' ,'<' and '>>' because they are redundant for the params of exec() 
*/


void redirect(char **arglist);
static void output_redirect(char*,int);
static void input_redirect(char*);
static void delete_io_symbol(char**,int);
void redirect(char **args){
	/*
	purpose: determines if io redirect symbol in cmd and process it
	details: it is necessary to delete the symbol in the args,or exec() would raise a error
	*/
        char *cp;
        int mode = 0;
        for(int i = 0;args[i];++i){
                if(strcmp(args[i],">") == 0){
                        *args[i] = '\0';
                        output_redirect(args[i + 1],mode);
                }
                else if(strcmp(args[i],">>")== 0){
                        mode = 1;
                        output_redirect(args[i + 1],mode);
                        delete_io_symbol(args,i);
                }
                else if(strcmp(args[i],"<") == 0)
                        input_redirect(args[i + 1]);
                else
                        continue;
                delete_io_symbol(args,i);
        }
}

void recover_stdio(){
	int ttyfd;
	ttyfd = open("/dev/tty",O_RDWR);
	dup2(ttyfd,0);
	dup2(ttyfd,1);
	dup2(ttyfd,2);
	
}

static void input_redirect(char *filename){
	/*
	details:remember to set access permission
	*/
        if(filename == NULL)
                return;
        int fd;
	if((fd = open(filename,O_RDONLY,0664)) < 0)
                perror("open file error");
        dup2(fd,0);
        close(fd);
        return;
}

static void output_redirect(char *filename,int mode){
	/*
	details:set the O_CREAT flag means if file not exsited then create it
	*/
        if(filename == NULL) return;
        int flag = mode ? (O_WRONLY | O_APPEND):O_WRONLY;
        int fd;
	if((fd = open(filename,flag | O_CREAT,0664)) < 0)
                perror("open file error");
        dup2(fd,1);
        close(fd);
        return;
}

static void delete_io_symbol(char **args,int loc){
        int j;
        for (j = loc;args[j + 2];++j)
                args[j] = args[j + 2];
        args[j] = NULL;
}

