#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        //printf("Найден путь %s\n", argv[1]);   
        FILE* readFile = fopen(argv[1], "r");
        char tmp[1024];
        //printf("Открыт файл\nПроисходит чтение файла:\n");
        while (fgets(tmp, 1024, readFile)) 
        {
            printf("%s", tmp);
        }      
    }
    else
    {
        //printf("Неверный ввод\nДля архивации: ./main {путь для архивации}\nДля разархивации: ./main {путь до файла result} {путь для разархивации}\n");
        return -1;
    }
  

    //printf("\nЧтение файла закончено\n");
    return 0;
}