#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int data=54;
int BSS;


void exo1() {
    //EXERCICE 1

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
    printf("Adresse de la fonction main : %p \n",exo1);
    printf("Adresse de la fonction printf : %p \n",printf);
    void *mmapVariable=mmap(NULL,4096,PROT_READ |PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    printf("Adresse de la mmap : %p \n",&mmapVariable);
    pid_t pid = fork();
    if (pid == 0) {
        execlp("pmap","pmap","-X",pid_main_str,(char *)NULL);
    }
    else if (pid > 0) {
        wait(NULL);
        munmap(mmapVariable,1024);
    }
    else {
        perror("Fork failed, lol");
    }

}

int exo2() {
    //EXERCICE 2
    int fd= open("../text.txt",O_RDWR);
    if (fd==-1) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    struct stat st;
    long sizeFile=0;
    if(stat("../text.txt",&st)==0) {
        sizeFile=st.st_size;
        printf("La taille du fichier est : %ld octets \n",sizeFile);
    }else {
        perror("stat");
    }
   char * mmapText=mmap(NULL,sizeFile,PROT_READ |PROT_WRITE,MAP_SHARED,fd,0);
    for (size_t i=0;i<sizeFile/2;i++) {
        char temp=mmapText[i];
        mmapText[i]=mmapText[sizeFile-i-1];
        mmapText[sizeFile-i-1]=temp;
    }
    //write(STDOUT_FILENO,mmapText,sizeFile);
    printf("%s \n",mmapText);
    munmap(mmapText,sizeFile);
    close(fd);

 /* ssize_t bytesRead;
    char buffer[256];
    while ((bytesRead=read(fd,buffer,sizeof(buffer)-1))>0) {
        buffer[bytesRead]='\0';
        printf("%s",buffer);

    }*/
    return 0;
}

typedef struct Node {
    int data;
    struct Node* ptrNext;
} Node;

Node buildList(int n) {
    Node head = NULL;
    if (n > 0) {
        Node newNode;
        head = newNode;
        Node memNode = NULL;
        for (int i=0; i<n;i++) {
            newNode.data = i;
            newNode.ptrNext = NULL;

            memNode.ptrNext = &newNode;
            memNode = newNode;
            Node newNode = NULL;
            printf("Node %i \n",i);
        }
    }
    return head;
}
void exo3() {
    Node* head = buildList(12);

}

int main(void)
{
    exo1();
    exo2();
    //exo3();
    return 0;
}