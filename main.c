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
    struct Node* next;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

Node* createList(int n) {
    Node* head = NULL;
    Node* temp = NULL;
    for (int i = 1; i <= n; i++) {
        Node* newNode = createNode(i);
        if (head == NULL) {
            head = newNode;
        } else {
            temp->next = newNode;
        }
        temp = newNode;
    }
    return head;
}

void displayList(Node* head) {
    while (head != NULL) {
        printf("%d => ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

int listLength(Node* head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

void displayListWithAddress(Node* head) {
    while (head != NULL) {
        printf("Adresse du maillon: %p, Valeur du maillon: %d\n", head, head->data);
        head = head->next;
    }
}

Node* removeFirst(Node* head) {
    if (head == NULL) return NULL;
    Node* temp = head;
    head = head->next;
    free(temp);
    return head;
}

Node* removeLast(Node* head) {
    if (head == NULL || head->next == NULL) {
        free(head);
        return NULL;
    }
    Node* temp = head;
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    return head;
}

Node* addLast(Node* head, int value) {
    Node* newNode = createNode(value);
    if (head == NULL) {
        return newNode;
    }
    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return head;
}

Node* addFirst(Node* head, int value) {
    Node* newNode = createNode(value);
    newNode->next = head;
    return newNode;
}

Node* concatenateLists(Node* list1, Node* list2) { // add the elements of list2 at the end of list1
    if (list1 == NULL) return list2;
    Node* temp = list1;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = list2;
    return list1;
}

Node* mapList(Node* head, int (*func)(int)) {
    Node* temp = head;
    while (temp != NULL) {
        temp->data = func(temp->data);
        temp = temp->next;
    }
    return head;
}

int square(int x) {
    return x * x;
}

/* Transform the linked list in a doubly linked list */

typedef struct doublyLinkedNode {
    int data;
    struct DoublyLinkedNode* prev;
    struct DoublyLinkedNode* next;
} DoublyLinkedNode;

DoublyLinkedNode* createDoublyLinkedNode(Node* simpleNode) {
    DoublyLinkedNode* newNode = (DoublyLinkedNode*)malloc(sizeof(DoublyLinkedNode));
    newNode->data = simpleNode->data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

DoublyLinkedNode* createDoublyLinkedChain(Node* head) {
    Node* temp = head;
    DoublyLinkedNode* doubleHead = createDoublyLinkedNode(head);
    DoublyLinkedNode* doubleTemp = doubleHead;
    DoublyLinkedNode* newDoubleNode = NULL;
    while (temp->next != NULL) {
        newDoubleNode = createDoublyLinkedNode(temp);
        newDoubleNode->prev = doubleTemp;
        doubleTemp->next = newDoubleNode;
        doubleTemp = newDoubleNode;
    }
    return doubleHead;
}

/* Create a doubly linked cycle list of n elements */

DoublyLinkedNode* createCycle(int n) {
    DoublyLinkedNode* head = createDoublyLinkedChain(createList(n));

    if (head == NULL || head->next == NULL) {
        return NULL;
    }

    DoublyLinkedNode* queue = head;
    while (queue->next != NULL) {
        queue = queue->next;
    }
    queue->next = head;
    head->prev = queue;
    return head;
}


void exo3() {
    printf("Votre liste chaînée est : \n");
    int n = 8;
    Node* list = createList(n);
    displayList(list);

}

int main(void)
{
    //exo1();
    //exo2();
    exo3();
    return 0;
}