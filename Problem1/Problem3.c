// Code by Zachary Waynor
// Project 1 Problem 3
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

    int IntCountL = 50;
    int num[IntCountL];
    int hiddenKeys[IntCountL];
    int hiddenKeyCount = 0;
    int max = 0;
    // int min = INFINITY;
    int sum = 0;
    int avg = 0;
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

    // Shared Memory for sum and max
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int shmid2 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    void *mem = shmat(shmid, NULL, 0);   // sum
    void *mem2 = shmat(shmid2, NULL, 0); // max
    // Shared Memory for Global Start


    int X = 3;
    int Np = 22;

    int APStart = 0;
    int APEnd = IntCountL / Np;
    int APSize = IntCountL / Np;
    int APGlobalEnd = IntCountL;

    int LpNum = 0;

    int shmid3 = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    void *ProcessCount = shmat(shmid3, NULL, 0); // sum
    *(int *)ProcessCount = 1;
    int ProcessNumber = 1;

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
        if (num[i] > max)
            max = num[i];
        sum = sum + num[i];
        if (num[i] < 0)
        {
            hiddenKeys[hiddenKeyCount] = i;
            hiddenKeyCount++;
            printf("Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), i);
        }
    }

    int status;
    for (int i = 0; i < X; i++)
    {
        wait(&status);
    }

    int sumChild = 0;
    int maxChild = 0;

    sumChild = *(int *)mem;
    maxChild = *(int *)mem2;
    sum = sum + sumChild;
    if (maxChild > max)
        max = maxChild;
    if (APStart > 0)
    {
        *(int *)mem = sum;
        *(int *)mem2 = max;
    }
    else
    {
        avg = sum / IntCountL;
        printf("Max = %d, Avg = %d \n\n", max, avg);
    }

    shmdt(mem);
    shmdt(mem2);
    shmdt(ProcessCount);

    fclose(fptr);
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
