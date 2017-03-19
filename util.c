#include"my_shell.h"
#include<string.h>
#include<pwd.h>
int is_legal_name(char *name){
        /*purposes:if name is legal name return 1 else 0*/
        char *cp;
        for(cp = name;*cp;cp++){
                if((isdigit(*cp) && cp == name) || !(isalnum(*cp) || *cp=='_'))
                        return 0;
        }
        return 1;
}

char **create_new_arglist(char **arglist){
	char **reslist;
	int len = 0;
	int i;
	for(i = 0;arglist[i];++i)++len;
	reslist = (char**)emalloc(sizeof(char*)*(len + 1));
	reslist[len] = 0;
	for(i = 0;arglist[i];++i){
		reslist[i] = emalloc(strlen(arglist[i]) + 1);
		strcpy(reslist[i],arglist[i]);
	}
	return reslist;
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

void freelist(char **arglist){
	for(int i =0;arglist[i];++i)
		free(arglist[i]);
	free(arglist);
}

void print_prompt(char *prompt){
	char path[MAXPATH];
	printf("%s:%s%s",getenv("LOGNAME"),getcwd(path,MAXPATH),prompt);
}
