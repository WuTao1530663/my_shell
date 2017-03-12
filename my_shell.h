#define is_delimiter(c) (((c) == ';') ||((c) == '\n'))
#define ARGLEN 20
#define MAXARGS 20
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
char *next_cmd(char*,FILE*);
char **splitline(char *);
void freelist(char**);
void *emalloc(size_t);
void *erealloc(void*,size_t);
int execute(char**);

