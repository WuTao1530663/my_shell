#include"my_shell.h"
#include<string.h>

/*
interface:
	var_store(name,value) interface of the builtin cmd '=' storing var to vars array 
	var_print_list() interface of the builtin cmd 'set' to print the all of vars
	var_export(name) interface ofthe builtin cmd 'export' to set a var as a environment var
	
environment related functions:
	var_init_from_environ() init the vars array from environ
	var_to_environ(environ) copy from the vars array to environ
*/

struct var{
	char *var; 
	int global;//1 for yes 0 for no
};

static struct var vars[MAXVARS];
static struct var* find_item(char *name,int first_blank);
static char* var_new_string(char *name,char *value);

int var_store(char *name,char *value){
	/*
	 *traverse vars array,if found item,replace it with value,else add a new item in the end
	 returns: 0 for ok ,1 for trouble just like cmd 

	*/
	int rv = 1;
	struct var *itemp;
	char *s;
	if((itemp = find_item(name,1)) != NULL&&
			(s= var_new_string(name,value))!=NULL){
		if(itemp->var)
			free(itemp->var);
		itemp->var = s;
		rv = 0;
	}
	return rv;
}

int var_parse_and_store(char *arg){
	/*
	purpose:parses the var and determines if the name is legal
	*/
	int rv;
	char *cp;
	cp = strchr(arg,'=');
	*cp = '\0';
	rv = is_legal_name(arg) ? var_store(arg,cp + 1):1;
	*cp = '=';
	return rv;
}

void var_print_list(){		
	/*
	purpose:print the vars array to stdout,global var with '*' in the head
	*/
	for(int i = 0;vars[i].var!=NULL ;++i){
		if(vars[i].global)
			printf("*%s\n",vars[i].var);
		else 
			printf("%s",vars[i].var);
	}
}

int var_export(char *name){
	/*
	purpose:if var in vars array set the var global else create a global var
	*/
	int rv;
	struct var *itemp;
	if((itemp = find_item(name,0)) != NULL){
		itemp->global = 1;
		rv = 0;
	}

	else if(var_store(name,"") == 0)
		rv = var_export(name);

	return rv;
}

int var_init_from_environ(char **env){
	/*
	purpose:init the vars array with environ array
	notes:set vars[i].global with 1
	*/
	int i;
	char *newstring;
	for(i =0;env[i];++i){
		if (i ==MAXVARS)
			return 1;
		newstring = malloc(strlen(env[i]) + 1);
		if(newstring == NULL)
			return 1;
		strcpy(newstring,env[i]);
		vars[i].var = newstring;
		vars[i].global = 1;
	}
	return 0;	
}

char** var_to_environ(){
	/*
	purpose:copy from vars with global tag to a environ array
	*/
	
	int i,j,count = 0;
	char **env;
	for(i = 0;i <MAXVARS &&vars[i].var != NULL;++i)
		if(vars[i].global == 1)
			count++;
	env = malloc((count + 1) * sizeof(char *));
	if(env == NULL)
		return NULL;
	for(i = 0,j = 0;i <MAXVARS &&vars[i].var != NULL;++i)
		if(vars[i].global == 1)
			env[j++] = vars[i].var;
	env[j] = NULL;
	return env;
	}
static char* var_new_string(char *name,char *value){
	/*
	purpose:alloc memory to store the new string
	*/
	char *retval;
	retval = malloc(strlen(name) + strlen(value) + 2);
	if(retval != NULL)
		sprintf(retval,"%s=%s",name,value);
	return retval;
}
static struct var* find_item(char *name,int first_blank){
	/*
	
	returns:if not found and first_blank == 1 return address of the first_blank else if first_blank == 0 return NULL
		if found return  address of the item
	notes:since there is no delete,a first blank is end of array
	*/
	int i;
	int len = strlen(name);
	char *s;
	for(i = 0;i <MAXVARS &&vars[i].var!=NULL;i++){
		s = vars[i].var;
		if(strncmp(s,name,len) == 0 && s[len] =='=')
			return &vars[i];		//find item
	}
	if(i < MAXVARS && first_blank)
		return &vars[i];

	return NULL;
}
