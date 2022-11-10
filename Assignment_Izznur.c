#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#define  PIN_LENGTH 4

void sigint_handler(int sig)
{
  printf("\nSTOP SYSTEM INTERUPTED\n");
}

void getPIN(char pin[PIN_LENGTH + 1])
{
  srand(getpid() + getpid());

  pin[0] = 49 + rand() % 7;

  for(int i=1; i<PIN_LENGTH; i++)
  {
    pin[i] = 48 + rand() % 7;
  }
  pin[PIN_LENGTH] = '\0';
}

int main (void){

    int pipefds[2];
    int pipefds2[2];
    char pin[PIN_LENGTH + 1];
    char buffer[PIN_LENGTH + 1];
    char s[100];
    void sigint_handler(int sig);
    
    signal(SIGINT, sigint_handler);
    
    pipe(pipefds);

    pid_t pid = fork();

    if(pid>0) //parent
    {
      getPIN(pin);
      close(pipefds[0]);
      write(pipefds[1], pin, PIN_LENGTH + 1);
     
      printf("Please enter your name\n");
      fgets(s, 100, stdin);
      write(pipefds2 [1], s, 100);
      close(pipefds2 [1]);

      exit(EXIT_SUCCESS);
    }
    if(pid==0) //child
    {
      wait(NULL);

      close(pipefds[1]);
      
      read(pipefds[0], buffer, PIN_LENGTH + 1);
      close(pipefds[0]);
      read(pipefds2[0], s, 100);
      close(pipefds2[0]);
      
      printf("Hello! %s", s);
      printf("Your number is '%s'. \n", buffer);
    }
    
  return 0;
  return EXIT_SUCCESS;
}