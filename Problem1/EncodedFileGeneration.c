

//Code by Zachary Waynor 
//Project 1 EncodedFileGeneration

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

   FILE *fptr;
   fptr = fopen("Secret_Code.txt","w");
    int IntCountL = 50;
   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
    srand(time(0));
  
    for(int i = 0; i<IntCountL ; i++){
        fprintf(fptr,"%d\n",rand() % 100);
    }
   fclose(fptr);

   return 0;
}


