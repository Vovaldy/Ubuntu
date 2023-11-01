#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

char *menu[] = {
 "ls ", "cat", "kill","quit","task", NULL,
};

int taskarray[64];

int pidno[64];

void ctrl_C(int sig){
    int j = 0;
    while(pidno[j]>0)
    {
        kill(pidno[j], 9);
        printf("\nПроцесс с pid %i завершился",pidno[j]);
        j++;
    }
    exit(0);
        
}

int getchoice(char *greet, char *choices[]) {
 int flag = 0;
 char **option;
 char *chosed; 
 char str[50];
 //char extentions[4] = "ls\n";
 do 
 {
    printf("Выбор: %s\n", greet);
    option = choices;
    while (*option) {
        printf("%s\n", *option);
        option++;
    }//Вывод меню

    do
    {
        chosed = fgets(str, 50, stdin);//Ввод параметра
        //printf("%s",chosed);

    }while(str[0] =='\n');

    option = choices;
    printf("Ты выбрал  %s", chosed);
    
    //
    if (strcmp(str,"ls\n")==0) {
        //запустить функцию отправляющую в лс
        pid_t pid = fork();
        if (pid == 0) {
        execlp("/bin/ls", "/bin/ls", "-lh", NULL);
        } else {
            waitpid(-1, &pid, 0);
            printf("ls функция успешно завершена\n\n");
        }
        flag = 1;
        
    }
    //
    if (strncmp(str,"cat",3)==0) {
        //запустить функцию отправляющую в cat
        //c0a1t2 3имяфайла 4 - хз
        
        
        char buffer[46];
        int tempi = 4;
        int tempj = 0;
        while(str[tempi]!='\n')
        {
            buffer[tempj]=str[tempi];
            tempi=tempi + 1;
            tempj=tempj + 1;
        }
        //printf("%s\n", buffer);
        int tempk =0;
        buffer[tempi-4]='\000';
        printf("Найден файл %s\n", buffer);
        pid_t pid = fork();
        if (pid == 0) {
            execlp("cat", "cat", buffer, NULL);
            
        } else {
            waitpid(-1, &pid, 0);
            printf("cat функция успешно завершена\n\n");
        }
        flag = 1;
        
    }
    //
    if(strcmp(str,"kill\n")==0)
    {
        pid_t prozes = fork();
        
        if (prozes == 0) {
        
        
        char pidline[1024];
        char *pid;
        int i =0;
        
        FILE *fp = popen("pidof yandex_browser","r");
        fgets(pidline,1024,fp);
        pid = strtok(pidline," ");
        while(pid != NULL)
                {

                        pidno[i] = atoi(pid);
                        pid = strtok (NULL , " ");
                        i++;
                }
        int j =0;
        printf("Начало закрытия процесса \n");
        
        ctrl_C(0);
    
        pclose(fp);
        } else {
            waitpid(-1, &prozes, 0);
            printf("\nkill функция успешно завершена\n\n");
        }
        flag=1;
    }
    //
    if (strcmp(str,"quit\n")==0) {
    flag = 1;
    }
    option++;
    //Выбор варианта
    
    if(strcmp(str,"task\n")==0)
    {
        pid_t pid_c;
        int i= 0;
        int taskn = 5;
     for(i = 0; i < taskn; i++)
     { 
          pid_t pid = fork();

        if (pid > 0)
        {
            /* parent process */
            pid_c = pid;
            pidno[i] = pid_c;
            if (i == taskn - 1)
            {
                ctrl_C(0);
                exit(0);
            }
        }
        else if (pid == 0)
        {
            /* child process. */
            pidno[i] = getpid();
            printf("Создан чилд проц %i       \n", pidno[i]);
            getchar();
            break;
        }
        else
        {
            /* error */
            exit(EXIT_FAILURE);
        }

    }

        flag =1;
    }


    if (!flag) { printf("Неверный выбор\n\n");}

 } while (!flag);//Печатаем менюшку и выводим всё пока не будет значение из нужных нам
 //
 return str[0];
}

int main() {
 int choice = 0;
 do {
    choice = getchoice("Пожалуйста выберите действие", menu);
    //printf("You have chosen: %c\n", choice);//Вы выбрали это
 } while (choice != 'q');

 exit(0);
}