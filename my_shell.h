#define is_delimiter(c) (((c) == ';') ||((c) == '\n'))
#define ARGLEN 20
#define MAXPIPES 10
#define MAXARGS 20
#define MAXVARS 300
#define MAXPATH 100
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>
//splitline.c
char *next_cmd(char*,FILE*);
char **splitline(char *);
void freelist(char**);

//process cmd
int execute(char**);
int process(char**);

//controlflow.c
int ok_to_execute();
int do_control_command(char**);
int is_control_command(char*);
int is_in_while_block();
//builtin.c

int is_built_in_command(char*);
int do_built_in_command(char**);

//var.c
int var_parse_and_store(char*);
int var_store(char*,char*);
int var_export();
void var_print_list();
int var_init_from_environ(char**);
char** var_to_environ();

//redirect.c
void redirect(char **);
void recover_stdio();

//pipe.c
void pipe_split_args(char **,int[],int*);
void pipe_create(int [][2],int);
void do_pipe(int [][2],int,int,int);

//util.c
int is_legal_name(char*);
void *emalloc(size_t);
void *erealloc(void*,size_t);
char **create_new_arglist(char**);
void print_prompt(char *prompt);
void freelist(char**);
