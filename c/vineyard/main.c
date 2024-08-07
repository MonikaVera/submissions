#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <wait.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <time.h>

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

void startBusesOnDay(char*, char*, char**);
void startBuses(char*, char*, char*);
int fogad(int, int);
int kuld(int, int, int);
int readPipe(char*);
void writeInPipe(char*, char*, char*, char*);
int numOfPassegers(char*);
void messageDevide(char*, char**, char**, int, int*, int*);
void getMessage(char*, char*, char**, int*);
void handler(int);

struct uzenet { 
    long mtype;
    char mtext [ 1024 ]; 
};

int main(int argc, char* argv[]) {
    srand(time(NULL));
    char num[MAXLENGTH];
    int maxNumOfWorkers[] = {5,12,2,3,4,0,0};
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
        printf("5. Busz indítása\n");
        printf("6. Kilépés\n");
    
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
            case 5: startBusesOnDay(argv[0], fileName, daysName);
                continue;
            case 6: printf("Viszlát!\n"); 
                continue;
            default: printf("Rossz választás! 1-6-ig írjon be számot!\n");
                continue;
        } 
        
    } while (choice != 6);
    
}

void startBusesOnDay(char* forKey, char* fileName, char** daysName) {
    char day[MAXLENGTH];
    printf("Adja meg, hogy melyik nap induljon a busz!\n");
    fgets(day,NAMELENGTH,stdin);
    for(int i=0; i<WEEKDAYS; i++) {
        if(contains(day,0,strlen(day),daysName[i])==0) {
            startBuses(forKey, fileName, daysName[i]);
        }
    }
}

void startBuses(char* forKey, char* fileName, char* day) {
    signal(SIGTERM,handler);
    signal(SIGUSR1,handler);
    int status;
    int pid;
    char pipename1[20];
    char pipename2[20];
    int uzenetsor;
    key_t kulcs;

    int randNum=(rand()%10000)+getpid();
    sprintf(pipename1,"/tmp/pipe1%d",randNum);
    int fid1=mkfifo(pipename1, S_IRUSR|S_IWUSR );
    if (fid1==-1)
    {
	printf("Error number: %i",errno);
	perror("Gaz van:");
	exit(EXIT_FAILURE);
    }
    sprintf(pipename2,"/tmp/pipe2%d",randNum+1);
    int fid2=mkfifo(pipename2, S_IRUSR|S_IWUSR );
    if (fid2==-1)
    {
	printf("Error number: %i",errno);
	perror("Gaz van:");
	exit(EXIT_FAILURE);
    }

    kulcs = ftok(forKey,1);  // kulcsot genrál fájlból
    //printf ("A kulcs: %d\n",kulcs);
    uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
    if ( uzenetsor < 0 ) { 
        perror("msgget"); 
        return; 
    }

    pid_t child=fork();
    if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
    if (child>0) //parent
    {
        pid_t child2=fork();
        if (child2<0){perror("The fork calling was not succesful\n"); exit(1);} 
        if (child2>0) //parent
        {
            pause(); //waits till a signal arrive 

            writeInPipe(pipename1, pipename2, fileName, day);
            sleep(5);
            
            fogad(uzenetsor, 2);
            fogad(uzenetsor, 1);
            status = msgctl( uzenetsor, IPC_RMID, NULL ); 
            if ( status < 0 ) 
               perror("msgctl"); 
            return;
            wait(NULL);
            
        } else { //child2
            char s[1024]="Semmi";		
            sleep(5);
            kill(getppid(),SIGTERM);
            printf("Második busz utasai: ");
	        int num=readPipe(pipename2);
            kuld(uzenetsor, 2, num);
            exit(0);
        }
    }
    else //child
    {
        char s[1024]="Semmi";		
        sleep(5);
        kill(getppid(),SIGUSR1); 
        printf("Első busz utasai: ");
        int num=readPipe(pipename1);
        kuld(uzenetsor, 1, num);
        exit(0);
    }
}

void handler(int signumber){
    if(signumber==SIGUSR1) {
        printf("Az 1. busz készen áll!\n");
    } else {
        printf("A 2. busz készen áll!\n");
    }
}

void getMessage(char* day, char* fileName, char** bus, int* count) {
    int f;
    f=open(fileName,O_RDONLY);
    char c;
    char temp[MAXLENGTH*5];
    int i=0;
    *count=0;
    while (read(f,&c,sizeof(c))) { 
        if(c=='\n') {
            if(contains(temp, 0, i, day)==0) {
                for(int k=0; k<i; k++) {
                    if(temp[k]==' ') {
                        (*bus)[*count]=temp[k];
                        ++(*count);
                        break;
                    } else {
                        (*bus)[*count]=temp[k];
                        ++(*count);
                    }
                }
            }
            i=0;
        } else {
            temp[i]=c;
            i++;
        }
    }
    close(f);
}

void messageDevide(char* Names, char** bus1, char** bus2, int count, int* lenghtOne, int* lengthTwo) {
    int count1 =0;
    int count2 =0;
    *lenghtOne=0;
    *lengthTwo=0;
    int from;
    int first=0;
    for (int l=0; l<count; l++) {
        if(first==0) {
            if(count1<5) {
                (*bus1)[l]=Names[l];
                ++(*lenghtOne);
                if(Names[l]==' ') {
                    ++(count1);
                }
            }
            if(count1==5) {
                from=l;
                first=1;
            } 
        } else if(first==1) {
            if(count2<5) {
                (*bus2)[l-from-1]=Names[l];
                ++(*lengthTwo);
                if(Names[l]==' ') {
                    ++(count2);
                }
            }
            if(count2==5) {
                first=2;
            } 
        }
        else {
            break;
        } 
    }
}

int numOfPassegers(char* Names) {
    int count=0;
    for(int i=0; i<strlen(Names); i++) {
        if(Names[i]==' ') {
            count++;
        }
    }
    if(count<5) {
        return count;
    } else {
        return 5;
    }
}

void writeInPipe(char* pipename1, char* pipename2, char* fileName, char* day) {
    char* bus=(char*)malloc(5*MAXLENGTH*sizeof(char));
    char* busOne=(char*)malloc(MAXLENGTH*sizeof(char));
    char* busTwo=(char*)malloc(MAXLENGTH*sizeof(char));
    int count, count1, count2;
    int fd1, fd2;
    getMessage(day, fileName, &bus, &count);
    messageDevide(bus, &busOne, &busTwo, count, &count1, &count2);

    fd1=open(pipename1,O_WRONLY|O_TRUNC);
    for(int i=0; i<count1; i++) {
        write(fd1,&(busOne[i]),sizeof(busOne[i]));
    }
    close(fd1);
    
    fd2=open(pipename2,O_WRONLY|O_TRUNC);
    for(int i=0; i<count2; i++) {
        write(fd2,&(busTwo[i]),sizeof(busTwo[i]));
    }
    /*if(count2!=0) {
        write(fd2,busTwo,strlen(busTwo));
    } else {
        write(fd2,"Nincs",strlen("Nincs"));
    }*/
    close(fd2);
    free(busOne);
    free(busTwo);
    free(bus);
}

int readPipe(char* pipename) {
    int fd;
    char c;
    int cur=0;
    fd=open(pipename,O_RDONLY|O_TRUNC);
        while(read(fd,&c,sizeof(c)) && cur<=5) {
            printf("%c",c);
            if(c==' ') {
                cur++;
            }
        }
    printf("\n");
    close(fd);
    return cur;
}

int kuld( int uzenetsor, int th, int num) 
{ 
    char strNum[MAXLENGTH];
    sprintf(strNum, "%d. busz uatasainak száma: %d", th, num);
    struct uzenet uz = { th, ""}; 
    strcpy(uz.mtext, strNum);

    int status; 
    status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    return 0; 
}

int fogad( int uzenetsor, int th ) 
{ 
    struct uzenet uz; 
    int status; 
    status = msgrcv(uzenetsor, &uz, 1024, th, 0 ); 
    if ( status < 0 ) 
        perror("msgsnd"); 
    else
        printf("%s\n", uz.mtext ); 
    return 0; 
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