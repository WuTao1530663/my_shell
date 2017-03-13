#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"my_shell.h"



//char *estrcpy(char *dst,const char *src);

char *next_cmd(char*prompt,FILE *fp){
	// purpose:read next command line from fp
	// returns:dynamically allocated string holding command line
	// errors:NULL at EOF
	// notes: allocates space in BUFSIZ chunks.
	char *buf;		
	int bufspace = 0;	
	int pos = 0;
	int c;
	printf("%s",prompt);
	while((c = getc(fp)) != EOF){
		if (pos + 1 >= bufspace){
			if(bufspace == 0)
				buf = emalloc(BUFSIZ);
			else
				buf = erealloc(buf,bufspace + BUFSIZ);
			bufspace +=BUFSIZ;
		}
		if(is_delimiter(c))
			break;
		buf[pos++] = c;
	}	
	if(c == EOF && pos == 0)
		return NULL;
	buf[pos] = '\0';
	return buf;
}

char **splitline(char *line){


	char **args = emalloc(sizeof(char*) * MAXARGS);
	int index = 0;
	char arg[ARGLEN];
	/*estrcpy(arg,strtok(line," "));
	args[index] =(char*) emalloc(strlen(arg) + 1);
	estrcpy(args[index++],arg);	
	while((estrcpy(arg,strtok(NULL," "))) != NULL){		
		args[index] =(char*) emalloc(strlen(arg) + 1);
		estrcpy(args[index++],arg);
		if(index == MAXARGS + 1)
			perror("too many args");
	}
	*/
	args[index++] = strtok(line," ");
	while((args[index++] = strtok(NULL," ")) != NULL);
	return args;

}


void *emalloc(size_t n){
	void *rv;
	if((rv = malloc(n))== NULL)
		perror("out of memory");
	return rv;
}

void *erealloc(void *p,size_t n){

	void *rv;
	if((rv = realloc(p,n)) == NULL)
		perror("realloc() failed");
	return rv;
}
/*
void freelist(char **list){
	//1st free char* in list then free char**
	char **cp = list;
	while(*cp)
		free(*cp++);
	free(list);
}

char * estrcpy(char *dst,const char*src){
	if(src == NULL) return NULL;
	else return strcpy(dst,src);
}*/
