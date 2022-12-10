/*Korišćenjem programskog jezika C napisati Linux program koji se deli u tri procesa. Prvi proces cita iz 
datoteke neparni.txt liniju po liniju i salje ih procesu roditelju korišćenjem datavoda, drugi iz datoteke 
parni.txt čita liniju po liniju I šalje ih procesu roditelju takođe koriščenjem datavoda. Proces roditelj 
obezbeđuje da se u datoteku spojeno.txt naizmenično upisuju neparne i parne linije.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
    int pd1[2]; 
    int pd2[2]; 
    int pid1,pid2;
   
    if(pipe(pd1)==-1)
    {
        printf("Javila se greska prilikom kreiranja prvog datavoda!");
        return -1;
    }

    if(pipe(pd2)==-1)
    {
        printf("Javila se greska prilikom kreiranja drugog datavoda!");
        return -1;
    }

    pid1=fork();

    if(pid1!=0) //Roditeljski proces
    {
        close(pd1[1]);
        close(pd2[1]);

        char LinijaS[80];

        FILE* fS=fopen("spojeno.txt","w");

        int CP,CN;

        while(1)
        {
            CN=read(pd2[0],LinijaS,80);
            
            if(CN==0)
            break;
            else
            {
                fprintf(fS,"%s",LinijaS);
            }

            CP=read(pd1[0],LinijaS,80);

            if(CP==0)
            break;
            else
            {
                fprintf(fS,"%s",LinijaS);
            }

        }

        fclose(fS);

        close(pd1[0]);
        close(pd2[0]); 

    }
    else
    {

        pid2=fork();

        if(pid2!=0)
        {
            // Ovaj proces deteta ptvara datoteku parni

            close(pd1[0]);
            close(pd2[0]);
            close(pd2[1]);

            FILE* fparni=fopen("parni.txt","r");

            char Linija[80];

            while(!feof(fparni))
            {
                fgets(Linija,80,fparni);
                write(pd1[1],Linija,80);
            }

            close(pd1[1]);

            //wait(NULL);
        }
        else
        {
            close(pd1[0]);
            close(pd1[1]);
            close(pd2[0]);

            char LinijaN[80];

            FILE* fneparni=fopen("neparni.txt","r");

            while(!feof(fneparni))
            {
                fgets(LinijaN,80,fneparni);
                write(pd2[1],LinijaN,80);
            }

            close(pd2[1]);
        }
    }

    return 0;
}
