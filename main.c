#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>

int data=54;
int BSS;



int main(void)
{

    int *heap=(int*)malloc(sizeof(int));
    int stack=0;

    const pid_t mainPid = getpid();
    char pid_main_str[10];
    sprintf(pid_main_str,"%d", mainPid);

    //printf("L'id du processus est : %i \n",getpid());
    printf("Valeur afficher pour Data : %p \n",&data);
    printf("Valeur afficher pour BSS : %p \n",&BSS);
    printf("Valeur afficher pour Str : %p \n","Hello World !");
    printf("Valeur afficher pour Heap : %p \n",&heap);
    printf("Valeur afficher pour Stack : %p \n",&stack);
    printf("Adresse de la fonction main : %p \n",&main);
    printf("Adresse de la fonction printf : %p \n",&printf);
    void *mmapVariable=mmap(NULL,4096,PROT_READ |PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    printf("Adresse de la mmap : %p \n",&mmapVariable);
    pid_t pid = fork();
    if (pid == 0) {
        execlp("pmap","pmap","-X",pid_main_str,(char *)NULL);
    }
    else if (pid > 0) {
        munmap(mmapVariable,1024);
        wait(NULL);
    }
    else {
        perror("Fork failed, lol");
    }

    return 0;
}
