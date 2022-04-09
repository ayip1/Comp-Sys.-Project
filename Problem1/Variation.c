// Code by Zachary Waynor
// Project 1 Variaition
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    clock_t begin = clock();
    FILE *fptr;
    fptr = fopen("Secret_Code.txt", "r");

    printf("Enter number of Integers For CodeFile:");
    int IntCountL = 5000;
    scanf("%d", &IntCountL);
    int num[IntCountL];
    int hiddenKeys[IntCountL];
    
    int max = 0;
    // int min = INFINITY;
    
    pid_t pid;

    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    for (int i = 0; i < IntCountL; i++)
    {
        fscanf(fptr, "%d", &num[i]);
    }
    fclose(fptr);
    FILE *fptr2;
   fptr2 = fopen("OutputVariation.txt","w+");

    int hiddenKeyMax = 4;


    
    int shmid = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0600);
   
    int *hiddenKeyIndex = shmat(shmid, NULL, 0);   // sum
    
    int shmid2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    void *hiddenKeyCount = shmat(shmid2, NULL, 0); // max


    int X = 5;
    int Np = 20;
    printf("Enter number of Children X For CodeFile:");
    scanf("%d", &X);

    printf("Enter number of Processces NP For CodeFile:");
    scanf("%d", &Np);

    int APStart = 0;
    int APEnd = IntCountL / Np;
    int APSize = IntCountL / Np;
    int APGlobalEnd = IntCountL;

    int LpNum = 0;

    int shmid3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    void *ProcessCount = shmat(shmid3, NULL, 0); // sum
    *(int *)ProcessCount = 1;
    int ProcessNumber = 1;

    for (int i = 0; i < hiddenKeyMax; i++){
            hiddenKeyIndex[i] = INFINITY;
    }



    do
    {
        for (int j = 0; j < X; j++)
        {
            if (j == 0 || pid > 0 && *(int *)ProcessCount < Np)
            {
                pid = fork();
                if (pid == 0)
                {

                    ProcessNumber = *(int *)ProcessCount + 1;
                    *(int *)ProcessCount = ProcessNumber;
                    APStart = (ProcessNumber - 1) * APSize;
                    APEnd = APStart + APSize;
                    if (ProcessNumber == Np)
                    {
                        APEnd = IntCountL;
                    }
                    else if (ProcessNumber > Np)
                    {
                        *(int *)ProcessCount = *(int *)ProcessCount - 1;
                        exit(0);
                    }
                }
            }
        }
    } while (pid == 0 && *(int *)ProcessCount < Np);

    printf("Hi I\'m process %d and my parent is %d. \n", getpid(), getppid());

    for (int i = APStart; i < APEnd; i++)
    {
        if(*(int *)hiddenKeyCount > hiddenKeyMax && i > hiddenKeyIndex[hiddenKeyMax-1]){
            i = APEnd;
        }
       
        if (num[i] < 0)
        {
            //hiddenKeys[hiddenKeyCount] = i;
            int hiddenKeyLoc = i;
            //printf("X\n");
            

            if(*(int *)hiddenKeyCount < hiddenKeyMax){
                *(int *)hiddenKeyCount = *(int *)hiddenKeyCount +1;
            
            }
            

            for (int j = 0; j < hiddenKeyMax; j++){
                if (hiddenKeyLoc > hiddenKeyIndex[hiddenKeyMax-1-j]){
                    j=hiddenKeyMax; 
                }
                else{
                    int temp = hiddenKeyIndex[hiddenKeyMax-1-j];
                    hiddenKeyIndex[hiddenKeyMax-1-j] = hiddenKeyLoc;
                    if (hiddenKeyMax-j < hiddenKeyMax){
                        hiddenKeyIndex[hiddenKeyMax-j] = temp;
                    }
                }
                //printf("%d\n",j);
            }
            
            

            //printf("Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), i);
        }
    }

    int status;
    for (int i = 0; i < X; i++)
    {
        wait(&status);
    }

    

    if (APStart  == 0){

        for (int i = 0; i < hiddenKeyMax; i++){
            fprintf(fptr2,"Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), hiddenKeyIndex[i]);
        }

    }
   
    shmdt(hiddenKeyIndex);
    shmdt(hiddenKeyCount);
    shmdt(ProcessCount);

    fclose(fptr2);
    // printf("TERMINATED\n");
    if (APStart  == 0)
    {
       
        shmctl(shmid, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);
        shmctl(shmid3, IPC_RMID, NULL);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("TotalExecutionTime = %f \n", time_spent);
    }

    exit(0);
}
