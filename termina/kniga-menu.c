#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

char *menu[] = {
 "a — add new record", "d — delete record", "q - quit", NULL,
};


int getchoice(char *greet, char *choices[]) {
 int chosen = 0;
 int selected;
 char **option;
 do 
 {
    printf("Choice: %s\n", greet);
    option = choices;
    while (*option) {
        printf("%s\n", *option);
        option++;
 }//Вывод меню

 do
 {
    selected = getchar();//Ввод параметра
 }while(selected =='\n');

 option = choices;
 while (*option) 
 {
    if (selected == *option[0]) {
    chosen = 1;
    break;
 }
 option++;
 }//Выбор варианта
 
 if (!chosen) {
 printf("Incorrect choice, select again\n");
 }

 } while (!chosen);//Печатаем менюшку и выводим всё пока не будет значение из нужных нам
 return selected;//
}

int main() {
 pid_t pid;
 int choice = 0;
 do {
    choice = getchoice("Please select an action", menu);
    printf("You have chosen: %c\n", choice);//Вы выбрали это
 } while (choice != 'q');

 exit(0);
}