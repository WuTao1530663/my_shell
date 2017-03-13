#include"my_shell.h"

int is_legal_name(char *name){
        /*purposes:if name is legal name return 1 else 0*/
        char *cp;
        for(cp = name;*cp;cp++){
                if((isdigit(*cp) && cp == name) || !(isalnum(*cp) || *cp=='_'))
                        return 0;
        }
        return 1;
}


