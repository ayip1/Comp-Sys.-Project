
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
 

    printf("Enter number of Integers L For CodeFile:");
    int IntCountL = 5000;
    scanf("%d", &IntCountL);
    int num[IntCountL];
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
    fclose(fptr);
    FILE *fptr2;
   fptr2 = fopen("Output1.txt","w+");
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

    fprintf(fptr2,"Hi I\'m process %d and my parent is %d. \n", getpid(),getppid());
    fprintf(fptr2,"Max = %d, Avg = %d \n\n",max, avg);
    
    for (int i=0;i<hiddenKeyCount ; i++){
        fprintf(fptr2,"Hi I\'m process %d and I found the hidden key in position A[%d]. \n", getpid(), hiddenKeys[i]);
    }
    
    fclose(fptr2);

    

    clock_t end = clock();
        double time_spent = (double)(end-begin)/ CLOCKS_PER_SEC;
        printf("TotalExecutionTime = %f \n", time_spent);


   return 0;
}