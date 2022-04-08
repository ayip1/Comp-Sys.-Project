#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main()
{  
  
   pid_t pidA = fork();
  
   if ( pidA == 0 )
   {
       printf("\nProcess A starting\n");
      
       pid_t pidB = fork();
      
       if ( pidB == 0 )
       {
           printf("\nProcess B starting\n");
          
           pid_t pidD = fork();
      
           if ( pidD == 0 )
           {
               int exitD = 10;
               printf("\nProcess D starting\n");
               sleep(1); //Allows child processes to terminate
               printf("\nProcess D returns %d\n",exitD);
               return exitD;
           }
           else
           {
               int exitB = 4;
               while(wait(NULL)>0); //Loop allows child processes to terminate
               printf("\nProcess B returning %d\n",exitB);
               return exitB;
           }
       }
       else
       {
           pid_t pidC = fork();
           if ( pidC == 0 )
           {
               int exitC = 6;
               printf("\nProcess C starting\n");
               sleep(2);
               printf("\nProcess C returns %d\n",exitC);
               return exitC;
           }
           else
           {
               int exitA = 2;
               while(wait(NULL)>0); //Loop allows child processes to terminate
               printf("\nProcess A returning %d\n\n",exitA);
               return exitA;
           }
       }
   }
   else
   {
       while(wait(NULL)>0); //Loop allows child processes to terminate
   }
}