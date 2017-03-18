#include"my_shell.h"
#include<string.h>
int is_built_in_command(char *arg){
	/*purpose: if arg is builtin cmd return 1 else 0*/
	return ((strcmp(arg,"set") == 0) || (strchr(arg,'=') != NULL) ||(strcmp(arg,"export")==0));
}

int do_built_in_command(char **args){
	/*purpose:deal with the builtin cmd*/
	int rv = 1;
	if(strcmp(args[0],"set") == 0){
		var_print_list();
	}
	else if(strchr(args[0],'=') != NULL){
		rv = var_parse_and_store(args[0]);
		if(rv == -1)
			perror("assign error");
	}
	else if(strcmp(args[0],"export")==0){
		if(args[1] != NULL && is_legal_name(args[1]))
			rv = var_export(args[1]);
		else {
			rv = -1;
			perror("export error");		
		}
	}
		
	return rv;
}



