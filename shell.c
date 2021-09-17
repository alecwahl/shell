#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 
#include<string.h>
#include<sys/wait.h>

char* getUsername(char* buf){
        char* tmp = getlogin();
	buf = malloc(256*sizeof(char));
	strcpy(buf,tmp);
}
int main() {
	const char* shellIdentifier = "AlecBash:";
	char* username;
	username = getUsername(username);
	while(1){
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("%s%s%s%s%s",username,"@",shellIdentifier,cwd,"$ ");
        	int curLength = 5;
        	char* buf = malloc(sizeof(char) * (curLength+1));
        	buf[0] = '\0';
        	char c = getchar();
        	int counter = 0;
        	while(c != '\n'){
                	buf[counter] = c;
                	counter += 1;
                	if(curLength == counter){
                        	buf = (char*) realloc(buf,sizeof(char*) * (1+curLength));
                        	curLength += 1;
                	}
			buf[counter+1] = '\0';
                	c = getchar();
       		}
		//printf("%s\n",buf);
		char* command = strtok(buf," ");
		if(strcmp(command,"exit") == 0){
                	free(buf);
			free(username);
			return 0;
              	}
		char* arguments = NULL;
		if(strcmp(command,"cd") == 0){
			arguments = strtok(NULL," ");
			//printf("%s",arguments);
			if (strlen(arguments) == 0 || chdir(arguments) != 0){
				printf("Could not change directory!\n");
			} 
		}
		else{
			arguments = strtok(NULL,"");
                        //printf("%s",arguments);

			char envPaths[2048];
			strcpy(envPaths,getenv("PATH"));
   			char* token = strtok(envPaths,":");
			int found = 0;
   			while(token != NULL) {
				//printf("\n");
				char path[1024] = {'\0'};
				//printf("%s\n",token);
				 
				strcpy(path,token);
				strcat(path,"/");
				strcat(path,command);
				//printf("%s\n",path);
				if(access(path,X_OK) == 0){
					
    					char **args;
					int argsLength = 3;
					args = malloc(argsLength * sizeof(char*));
					int status;
					if(arguments != NULL && strlen(arguments) != 0){
						int elcount = 1;
						//printf("%s",arguments);
						char* tmp = strtok(arguments," ");
						while(tmp != NULL) {
							//printf("%s\n",tmp);
							args[elcount] = tmp;
							elcount += 1;
							tmp  = strtok(NULL," ");
							if(elcount == argsLength){
								args = (char**)realloc(args,(argsLength + 1) * sizeof(char*));
								argsLength++;
							}
						}
						//printf("%d,%d",elcount,argsLength);
    						args[0] = path;
						//printf("%s",path);
						//args[1] = arguments;
    						args[argsLength-1] = NULL;
						//for(int i = 0;i<argsLength-1;i++){
						//	printf("%s\n",args[i]);
						//}
					}else{
						//printf("%s",path);
                                                args[0] = path;
                                                args[1] = NULL;
					}
    					if (fork() == 0){
        					execv(args[0], args);
					}else{
						wait(&status);
					}
					free(args);
					found = 1;
					break;
				}else{
					token = strtok(NULL, ":");
				}
			}
                        if(found == 0){
                        	printf("Command Not Found!\n");
                        }
			free(buf);
			//printf("\n");
		}
	}
}
