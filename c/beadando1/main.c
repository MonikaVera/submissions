#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

#define INIT 4
#define MAXLENGTH 100
#define NAMELENGTH 20
#define WEEKDAYS 7

int addDay(int*, int*, char*, char*, int);
void writeString(char*, int);
void readFromFile(char**, char*, int*);
void list(char*);
void readIn(char*, int*, int*, char**);
int contains(char*, int, int, char*);
void findFromTo(int*, int*, char*, char*, int);
void delete(char*, int*, char**);
void modify(char*, int*, int*, char**);

int main() {
    char num[MAXLENGTH];
    int maxNumOfWorkers[] = {5,6,2,3,4,0,0};
    int currentNumOfWorkers[] = {0,0,0,0,0,0,0};
    char* fileName="a.txt";
    char* daysName[] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};

    int f;
    f=open(fileName, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
    close(f);

    int choice;
    do {
        printf("\nMenu\n");
        printf("1. Új személy hozzáadása\n");
        printf("2. Listázás\n");
        printf("3. Napok hozzáadása\n");
        printf("4. Napok törlése\n");
        printf("5. Kilépés\n");
    
        fgets(num,MAXLENGTH,stdin);
        *strchr(num, '\n') = '\0';
        choice=atoi(num);

        switch (choice) {
            case 1: readIn(fileName, maxNumOfWorkers, currentNumOfWorkers, daysName);
                continue;
            case 2: list(fileName);
                continue;
            case 3: modify(fileName, maxNumOfWorkers, currentNumOfWorkers, daysName);
                continue;
            case 4: delete(fileName, currentNumOfWorkers, daysName);
                continue;
            case 5: printf("Viszlát!\n"); 
                continue;
            default: printf("Rossz választás! 1-5-ig írjon be számot!\n");
                continue;
        } 
        
    } while (choice != 5);
}

int addDay(int* maxNumOfWorkers, int* currentNumOfWorkers, char* arr, char* weekDay, int num) {
    if(strstr(arr, weekDay)!=NULL && maxNumOfWorkers[num]>currentNumOfWorkers[num]) {
        ++(currentNumOfWorkers[num]);
        return 0;
    }
    return 1;
}

void writeString(char* str, int g) {
    for(int i=0; i<strlen(str); i++) {
        if (write(g,&(str[i]),sizeof(str[i]))!=sizeof(str[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
    } 
    char c=' ';
    if (write(g,&c,sizeof(c))!=sizeof(c)) 
      {perror("There is a mistake in writing\n");exit(1);}
}

void readFromFile(char** str, char* fileName, int* sizeCur) {
    int f;
    int sizeMax=INIT;
    *sizeCur=0;
    char c;
    char* arr=(char*)malloc(sizeMax*sizeof(char));

    f=open(fileName,O_RDONLY);
    lseek(f,0,SEEK_SET);

    while (read(f,&c,sizeof(c))) { 
        if(sizeMax<=*sizeCur) {
            sizeMax*=2;
            arr=(char*)realloc(arr, sizeMax*sizeof(char));
        }
        arr[*sizeCur]=c;
        ++(*sizeCur);
    }
    *str=arr;
    close(f);
};

void list(char* fileName) {
    int f;
    f=open(fileName,O_RDONLY);
    char c;
    lseek(f,0,SEEK_SET);
    while (read(f,&c,sizeof(c))) { 
        printf("%c", c);
    }
    close(f);
}

void readIn(char* fileName, int* maxNumOfWorkers, int* currentNumOfWorkers, char** daysName) {
    printf("Adja meg a nevét (egyben, szóköz nélkül) és a napokat (szóközzel elválasztva)!\n");
    int g;
    char arr[MAXLENGTH];
    fgets(arr,MAXLENGTH,stdin);
    g=open(fileName, O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR);

    for(int i=0; ; i++) {
        if (write(g,&(arr[i]),sizeof(arr[i]))!=sizeof(arr[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
        if(arr[i]==' ' || arr[i]=='\n' || arr[i]=='\0') {
            break;
        }
    }

    for(int i=0; i<WEEKDAYS; i++) {
        if(addDay(maxNumOfWorkers, currentNumOfWorkers, arr, daysName[i], i)==0) writeString(daysName[i], g);
    }
   
    char c='\n';
    if (write(g,&c,sizeof(c))!=sizeof(c)) 
      {perror("There is a mistake in writing\n");exit(1);}
    //lseek(g,0,SEEK_SET);
    close(g);
    
};

int contains(char* fileText, int from, int to, char* day) {
    int j=0;
    for(int i=from; i<to; i++) {
        if(fileText[i]==day[j]) {
            j++;
        } else {
            j=0;
        }
        if(j==strlen(day)) {
            return 0;
        }
    }
    return 1;
}

void findFromTo(int* startName, int* endline, char* Name, char* fileText, int sizeCur) {
    *startName=0;
    int j=0;
    for(int i=0; i<sizeCur; i++) {
        if(fileText[i]==Name[j]) {
            j++;
        } else {
            *startName=i+1;
            j=0;
        }
        if(j==strlen(Name)) {
            break;
        }
    }

    *endline=*startName;
    for(int i=*startName; i<sizeCur; i++) {
        if((fileText[i])=='\n') {
            break;
        }
        ++*endline;
    }
}

void delete(char* fileName, int* currentNumOfWorkers, char** daysName) {
    int g;
    char Name[NAMELENGTH];
    char toDelete[MAXLENGTH];

    printf("Először adja meg a nevét (egyben, szóköz nélkül)!\n");
    fgets(Name,NAMELENGTH,stdin);
    *strchr(Name, '\n') = '\0';
    printf("Adja meg a törlendő napokat (szóközzel elválasztva)!\n");
    fgets(toDelete,MAXLENGTH,stdin);

    char* fileText;
    int sizeCur;
    readFromFile(&fileText, fileName, &sizeCur);
    int startName;
    int endline;
    findFromTo(&startName, &endline, Name, fileText, sizeCur);

    int deleteAll=0; //bool
    //ha a név nincs a fájlban, akkor nem írjuk bele
    //ha minden napot kitörlünk, akkor nem marad benne a mév
    if(startName!=sizeCur) {
        for(int i=0; i<WEEKDAYS; i++) {
            if(!((contains(fileText, startName, endline, daysName[i])==0
            && contains(toDelete, 0, strlen(toDelete), daysName[i])==0)
            || (contains(toDelete, 0, strlen(toDelete), daysName[i])==1
            && contains(fileText, startName, endline, daysName[i])==1))) {
            deleteAll=1;
            }
        }
    }    
  
    g=open(fileName, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
    for(int i=0; i<startName; i++) {
        if (write(g,&(fileText[i]),sizeof(fileText[i]))!=sizeof(fileText[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
    } 

    if(deleteAll==1) {
        writeString(Name, g);

        for(int i=0; i<WEEKDAYS; i++) {
            if(contains(fileText, startName, endline, daysName[i])==0) {
                if(contains(toDelete, 0, strlen(toDelete), daysName[i])==1) {
                    writeString(daysName[i], g);
                } else {
                    --(currentNumOfWorkers[i]);
                }
            } 
        }
        char c='\n';
        if (write(g,&c,sizeof(c))!=sizeof(c)) 
        {perror("There is a mistake in writing\n");exit(1);}
    }

    for(int i=endline+1; i<sizeCur; i++) {
        if (write(g,&(fileText[i]),sizeof(fileText[i]))!=sizeof(fileText[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
    }

    close(g);
    free(fileText);
}

void modify(char* fileName, int* maxNumOfWorkers ,int* currentNumOfWorkers, char** daysName) {
    int g;
    char Name[NAMELENGTH];
    char toAdd[MAXLENGTH];
    printf("Először adja meg a nevét (egyben, szóköz nélkül)!\n");
    fgets(Name,NAMELENGTH,stdin);
    *strchr(Name, '\n') = '\0';
    printf("Adja meg, hogy melyik napokat szeretné hozzáadni (szóközzel elválasztva)!\n");
    fgets(toAdd,NAMELENGTH,stdin);

    char* fileText;
    int sizeCur;
    readFromFile(&fileText, fileName, &sizeCur);
    int startName;
    int endline;
    findFromTo(&startName, &endline, Name, fileText, sizeCur);

    g=open(fileName, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
    for(int i=0; i<startName; i++) {
        if (write(g,&(fileText[i]),sizeof(fileText[i]))!=sizeof(fileText[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
    } 

    writeString(Name, g);
    for(int i=0; i<WEEKDAYS; i++) {
        if(contains(fileText, startName, endline, daysName[i])==0) {
            writeString(daysName[i], g);
            
        } else {
            if(contains(toAdd, 0, strlen(toAdd), daysName[i])==0 && maxNumOfWorkers[i]!=currentNumOfWorkers[i]) {
                ++(currentNumOfWorkers[i]);
                writeString(daysName[i], g);
            }
        }
    }
    char c='\n';
    if (write(g,&c,sizeof(c))!=sizeof(c)) 
        {perror("There is a mistake in writing\n");exit(1);}

    for(int i=endline+1; i<sizeCur; i++) {
        if (write(g,&(fileText[i]),sizeof(fileText[i]))!=sizeof(fileText[i])) 
        {perror("There is a mistake in writing\n");exit(1);}
    }

    close(g);
    free(fileText);
}