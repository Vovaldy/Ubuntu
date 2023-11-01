#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LENGTH 550

char *consoleComands[]= {"./readFile /home/vladimir/Molitva/linux/lr4MejProzVzaim/test.txt",
"./readFile /home/vladimir/Molitva/linux/lr4MejProzVzaim/test2.txt"};

char *vizovReadFile(int j) {
 FILE *read_fp;
 char* buffer = malloc(LENGTH);
 read_fp = popen(consoleComands[j], "r");
 if (read_fp != NULL) 
 {
 fread(buffer, sizeof(char), LENGTH, read_fp);
 pclose(read_fp);
 return buffer;
 }
 else{  exit(EXIT_FAILURE); }
}

int main()
{
    char *temp1;
    char *temp2;
    FILE *result;
    printf("\n\nФайл текст1\n");
    
    temp1 = vizovReadFile(0);
    printf("\n%s\n",temp1);
    printf("\nФайл текст2\n");
    temp2 = vizovReadFile(1);
    printf("\n%s\n", temp2);

    char xor[LENGTH];
    int i;

    result = fopen("/home/vladimir/Molitva/linux/lr4MejProzVzaim/result.txt", "w");
    for(i=0; i<LENGTH; ++i)
    {   xor[i] = (temp1[i] ^ temp2[i]); }
    
    //xor = (temp1 ^ temp2);
    printf("\nXOR побыйтовый вывод:\n");
    for(i=0; i<LENGTH; ++i)
        printf("%02X ", xor[i]);
    printf("\n");
    
    for(i=0; i<LENGTH; ++i)
    {   
        if (xor[i]=='\000')
        {   xor[i]=' ';}
        else 
        {
            xor[i]= temp1[i];
        }
    }
    fputs(xor, result);
}