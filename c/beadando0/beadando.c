#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "beadando.h"

int reverseThem(int argc, char **argv)
{ 
    int lineCount=0;
    int *ptr=&lineCount;
    if(argc < 2)
    {
        char ** words=beolvas(ptr, stdin);
        calling(words, ptr);
    }
    else
    {
        int canopenAll = 1;
        for(int i=1; i<argc; i++)
        {
            if(fopen(argv[i],"r")==NULL)
            {
                printf("File opening unsuccessful!\n");
            }
            else
            {
                canopenAll = 0;
                FILE* fp = fopen(argv[i],"r");
                char ** words=beolvas(ptr, fp);
                fclose(fp);
                calling(words, ptr);
                *ptr=0;
            } 
        }  
        if(canopenAll==1)
        {
            exit(-1);
        } 
    }
    return 0;
}

char** beolvas(int *ptr, FILE* fp)
{
    int size = INIT;
    char** words=(char**)malloc(size*sizeof(char*));

    if(!words)
    {
        printf("Memory allocation failed!\n");
        exit(-2);
    }
    char temp[SOR];
    
     while(fgets(temp,SOR,fp))
        {
                *strchr(temp, '\n') = '\0';
                if(*ptr >= size)
                {
                    size *= 2;
                    words = (char**)realloc(words, size * sizeof(char*));
                    if(!words)
                    {
                        printf("Memory allocation was unsuccesful!\n");
                        exit(-2);
                    }
                }
                char* currentLine = (char*)calloc(strlen(temp), sizeof(char));
                if(!currentLine)
                {
                    printf("Memory allocation was unsuccesful!\n");
                    exit(-2);
                }
                strcpy(currentLine, temp);
                words[*ptr] = currentLine;
                ++*ptr;
        }         
        return words;
}

void reverse(char ** words, int currentLine)
{
    
    for(int i=0; i< currentLine/2; i++)
    {
        char* seged = words[i];
        words[i]=words[currentLine-i-1];
        words[currentLine-i-1]= seged;   
    }
    
    for(int i=0; i< currentLine; i++)
    {
       for(int j=0; j<strlen(words[i])/2; j++)
       {
           char seged2 = words[i][j];
           words[i][j]=words[i][strlen(words[i])-j-1];
           words[i][strlen(words[i])-j-1]= seged2;
        }  
    }
     
}

void print(char ** fruit, int k)
{
    int kiSzam=k;
    for(int i=0; i< k; i++)
    {
        printf("%d ", kiSzam);
        printf("%s\n", fruit[i]);
        free(fruit[i]);
        kiSzam--;
    }
    free(fruit);
}

void calling(char ** words, int *ptr)
{
    reverse(words, *ptr);
    print(words, *ptr);
}
