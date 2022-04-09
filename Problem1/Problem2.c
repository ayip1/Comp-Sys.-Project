//Code by Zachary Waynor 
//Project 1 Problem 2
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
  fptr = fopen("Secret_Code.txt","r");
 
   
   int currentCreatedProcesses= 1;

   int numberOfChildrenX = 3;

      printf("Enter number of Integers L For CodeFile:");
    int IntCountL = 5000;
    scanf("%d", &IntCountL);

    printf("Enter number of Processces NP For CodeFile:");
    int numberOfProcesses = 12;
    scanf("%d", &numberOfProcesses);

   int num[IntCountL];
   int hiddenKeys[IntCountL];
   int hiddenKeyCount = 0;
   int max = 0;
   //int min = INFINITY;
   int sum = 0;
   int avg = 0;
   pid_t pid;
 

  if(fptr == NULL)
  {
     printf("Error!");  
     exit(1);            
  }
 
   for (int i=0;i<IntCountL; i++){
       fscanf(fptr,"%d", &num[i]);
   }
    fclose(fptr);
    FILE *fptr2;
   fptr2 = fopen("Output2.txt","w+");
   
   int StartPoint = 0;
  
   int AmountDistributed = IntCountL/numberOfProcesses;
 
   int EndPoint = AmountDistributed;
 
    //Shared Memory for sum
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int shmid2 = shmget(IPC_PRIVATE, sizeof(int) , IPC_CREAT | 0600);
    void *mem = shmat(shmid, NULL, 0);
    void *mem2 = shmat(shmid2, NULL, 0);
   do{
       pid = fork();
       currentCreatedProcesses++;
       if (pid < 0){ //error
           printf("Error!");  
           exit(1);      
       }
        
 
 
 
 
       else if (pid ==0){   //child process

           StartPoint = StartPoint + AmountDistributed;
      
 
           if (currentCreatedProcesses != numberOfProcesses){
               EndPoint = EndPoint + AmountDistributed;
           }
            else{
               EndPoint = IntCountL;
               fprintf(fptr2,"Hi I\'m process %d and my parent is %d. \n", getpid(),getppid());
 
               for (int i=StartPoint ;i<EndPoint; i++){
                   if (num[i] > max) max = num[i];
                   sum = sum + num[i];
                   if (num[i] < 0){
                       hiddenKeys[hiddenKeyCount] = i ;
                       hiddenKeyCount++;
                       fprintf(fptr2,"Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), i);
                   }
                  
               }
                //printf("Sum Before %d \n", sum);
                *(int *)mem = sum; 
                *(int *)mem2 = max; 

           }
       }
 
       else{//parent process
           printf("Hi I\'m process %d and my parent is %d. \n", getpid(),getppid());
 
           for (int i=StartPoint ;i<EndPoint; i++){
               if (num[i] > max) max = num[i];
               sum = sum + num[i];
               if (num[i] < 0){
                   hiddenKeys[hiddenKeyCount] = i ;
                   hiddenKeyCount++;
                   fprintf(fptr2,"Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), i);
               }
 
           }

           int status;
           waitpid(pid,&status,WUNTRACED);
           //printf("Hi I\'m process %d and I am done waiting. \n", getpid());
         

           int sumChild=0;
           int maxChild=0;
           //printf("Sum Before %d \n", sum);
           sumChild = *(int *)mem;
           maxChild = *(int *)mem2;
           //printf("Sum Child %d \n", sumChild);
           //printf("Max Child %d \n", maxChild);
           sum = sum + sumChild;
           if (maxChild > max) max = maxChild;
           //printf("Sum After %d \n", sum);
            if (StartPoint>0){
               *(int *)mem = sum; 
               *(int *)mem2 = max; 
            }
           else{
                avg = sum/IntCountL;
                fprintf(fptr2,"Max = %d, Avg = %d \n\n",max, avg);
           }
           
           
 
           
     
       }
      
   }while (pid==0 && currentCreatedProcesses < numberOfProcesses);




 
 
 
   //printf("Hi I\'m process %d and my parent is %d. \n", getpid(),getppid());
   //printf("Max = %d, Avg = %d \n\n",max, avg);
   //for (int i=0;i<hiddenKeyCount ; i++){
   //    printf("Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), hiddenKeys[i]);
   //}
  
   shmdt(mem);
   shmdt(mem2);
   fclose(fptr2);
   //printf("TERMINATED\n");
    if( StartPoint == 0){
        shmctl(shmid, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);

        clock_t end = clock();
        double time_spent = (double)(end-begin)/ CLOCKS_PER_SEC;
        printf("TotalExecutionTime = %f \n", time_spent);
        
    }
    

 
  exit(0);
}

