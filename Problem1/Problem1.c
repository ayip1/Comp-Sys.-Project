
//Code by Zachary Waynor 
//Project 1 Problem 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main()
{
   clock_t begin = clock();
   FILE *fptr;
   fptr = fopen("Secret_Code.txt","r");

    int IntCountL = 50;
    int num[50];
    int hiddenKeys[IntCountL];
    int hiddenKeyCount = 0;
    int max = 0;
    //int min = INFINITY;
    int sum = 0;
    int avg = 0;
   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
    for (int i=0;i<IntCountL; i++){
        fscanf(fptr,"%d", &num[i]);
    }


    for (int i=0;i<IntCountL; i++){
        if (num[i] > max) max = num[i];
        //if (num[i] < min) min = num[i];
        sum = sum + num[i];
        

        if (num[i] < 0){
            hiddenKeys[hiddenKeyCount] = i ;
            hiddenKeyCount++;
            
        }


    }
    avg = sum/IntCountL;

    printf("Hi I\'m process %d and my parent is %d. \n", getpid(),getppid());
    printf("Max = %d, Avg = %d \n\n",max, avg);
    for (int i=0;i<hiddenKeyCount ; i++){
        printf("Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), hiddenKeys[i]);
    }
    


    fclose(fptr);

    clock_t end = clock();
        double time_spent = (double)(end-begin)/ CLOCKS_PER_SEC;
        printf("TotalExecutionTime = %f \n", time_spent);


   return 0;
}