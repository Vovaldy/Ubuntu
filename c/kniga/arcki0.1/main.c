#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>


char* concat(char *s1, char *s2)
{

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char *result = malloc(len1 + len2 + 1);

    if (!result)
    {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

void archivizing(char *dir, int depth, FILE* archivePath)
{

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int flag = 0;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    chdir(dir);

    while ((entry = readdir(dp)))
    {

        lstat(entry->d_name, &statbuf); // запись в statbaf информации о файле

        if (S_ISDIR(statbuf.st_mode))
        {
            /* Находит каталог, но игнорирует . и .. */
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;

            //printf("%*s%s/\n", depth, "", entry->d_name);

            /* Рекурсивный вызов с новый отступом */
            archivizing(concat(concat(dir, "/"), entry->d_name), depth + 4, archivePath);
        }
        else if (strcmp(entry->d_name, "result")) {
            printf("%*s%s- Найден файл - ", depth, " ", entry->d_name);
            
            FILE* myfile = fopen(entry->d_name, "r");

            char tmp[1024];
            char c;

            char* pathToFile = concat(concat(dir, "/"), entry->d_name);
            printf("%s\n", pathToFile);

            int i=1;
            for (c = getc(myfile); c != EOF; c = getc(myfile))
            {
                if (c == '\n') // Increment count if this character is newline
                    {i = i + 1;}
            }
            //printf("%d\n",i);
            char size[10];
            sprintf(size, "%d", i);
            fputs(size,archivePath);
            fputs("\n",archivePath);
            fputs(entry->d_name,archivePath);
            fputs("\n",archivePath);
            //fputs(pathToFile,archivePath);
            
            fclose(myfile);
            
            myfile = fopen(entry->d_name, "r");
            while (fgets(tmp, 1024, myfile)) 
            {
                fputs(tmp, archivePath);
            }
                        
            fputs("\n", archivePath);
            fclose(myfile);
            //remove(entry->d_name);
            flag = 1;
        }
    }

    chdir("..");
    
    closedir(dp);

}

void unarchivizing(char* archivePathChar, char* parthTo) {

    FILE* archivePath = fopen(archivePathChar, "r");
    char tmp[1024];
    char* size;
    char* fileName;
    char* destinationFolder;
    DIR *dp;
        if ((dp = opendir(parthTo)) == NULL)
    {
        fprintf(stderr, "Не смог открыть директорию: %s\n", parthTo);
        return;
    }
    //char* destinationFolder = parthTo;
    while (1) {
        //printf("\n%s",tmp);
        size = fgets(tmp,1024,archivePath);
        if(size==NULL)
        {
            break;
        }
        int size_int = atoi(size);
        printf("\n\n Размер файла - %d",size_int);

        fileName = fgets(tmp,1024,archivePath);
        printf("\n Имя файла - %s",fileName);
        //printf("!!");
        destinationFolder = concat(parthTo,fileName);
        //destinationFolder = fgets(tmp,1024,archivePath);

        //fgets(tmp,1024,archivePath);
        //printf("\n%s",parthTo);
        destinationFolder[strlen(destinationFolder)-1] = 0;
        printf(" Место разахивации - %s",destinationFolder);
        //break;
        FILE* tmpFile = fopen(destinationFolder, "w+");
        //value
        int i = 0;
        while (i!=size_int) {
            
            fgets(tmp, 1024, archivePath);
            printf("\nПрогресс разархивации %d",i);
            fputs(tmp, tmpFile);
            i++;
        }
        
        fclose(tmpFile);
        //end value
        //break;//придумать зациклинность
    }

    

    fclose(archivePath);
    //remove(archivePathChar);
    printf("\nРазархивация завершена");
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Неверный ввод\nДля архивации: ./main {путь для архивации}\nДля разархивации: ./main {путь до файла result} {путь для разархивации}\n");
        return -1;
    }

    char* result = concat(argv[1],"/result"); 
    //char* resultDir = concat(argv[1]," | arki2.0");
    if (argc == 2)
    {
        printf("Найден путь %s\n", argv[1]);
        
        FILE* archivePath = fopen(result, "w+");
    
        archivizing(argv[1], 0, archivePath);
        fclose(archivePath);
        printf("Архив создан");
    }
    else if (argc ==3)
    {
        //int choice = atoi(argv[3]);
        //char* result = concat(argv[1], "/result");
        //if (choice == 0)
        char* destFolder = concat(argv[2],"/");
        printf("%s",destFolder);
        unarchivizing(result, destFolder); 
        //fclose(archivePath);
        //}
    }

    
    
    printf("\ndone\n");

    return 0;
}