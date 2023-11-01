#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<string.h>


pid_t child_id;
int pidno[64];

void ctrl_C(int sig){
    int j = 0;
    while(pidno[j]>0)
    {
        kill(pidno[j], 9);
        printf("\n You pressed Ctrl + C,Good Bye \n");
        j++;
    }
        
}


int main()
{
        char pidline[1024];
        char *pid;
        int i =0;
        
        FILE *fp = popen("pidof yandex_browser","r");
        fgets(pidline,1024,fp);

        printf("%s",pidline);
        pid = strtok(pidline," ");
        while(pid != NULL)
                {

                        pidno[i] = atoi(pid);
                        printf("%d\n",pidno[i]);
                        pid = strtok (NULL , " ");
                        i++;
                }
        int j =0;
        printf("I am Programmer \n");
        pid_t child_id = fork();
    if (child_id == 0) {
        signal(SIGINT, ctrl_C);
    }

        getchar();


    
    

        pclose(fp);
}
