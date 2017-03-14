#define is_delimiter(c) (((c) == ';') ||((c) == '\n'))
#define ARGLEN 20
#define MAXARGS 20
#define MAXVARS 300
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

//alloc and free memory
void freelist(char**);
void *emalloc(size_t);
void *erealloc(void*,size_t);

//process cmd
int execute(char**);
int process(char**);

//controlflow.c
int ok_to_execute();
int do_control_command(char**);
int is_control_command(char*);

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
void redirect(char **arglist);
void recover_stdio();

//util.c
int is_legal_name(char*);
