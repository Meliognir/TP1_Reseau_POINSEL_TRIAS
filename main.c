#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


/*---------------------EXERCICE 1----------------------*/

int data=54;
int BSS;

void exo1() {

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

/*---------------------EXERCICE 2----------------------*/

int exo2() {
    int fd= open("../text.txt",O_RDWR);
    if (fd==-1) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    struct stat st;
    long sizeFile=0;
    if(stat("../text.txt",&st)==0) { //this format worked with Clion
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

/*---------------------EXERCICE 3----------------------*/

/* Creation of a linked list */

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

/* Display the elements of the list */

void displayList(Node* head) {
    while (head != NULL) {
        printf("%d => ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

/* Find the length of the list */

int listLength(Node* head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

/* Display the elements of the list and their address */

void displayListWithAddress(Node* head) {
    while (head != NULL) {
        printf("Adresse du maillon: %p, Valeur du maillon: %d\n", head, head->data);
        head = head->next;
    }
}

/* Remove the first element of the list */

Node* removeFirst(Node* head) {
    if (head == NULL) return NULL;
    Node* temp = head;
    head = head->next;
    free(temp);
    return head;
}

/* Remove the last element of the list */

Node* removeLast(Node* head) {
    if (head == NULL || head->next == NULL) {
        free(head);
        return NULL;
    }
    Node* temp = head;
    Node* lastElement = temp->next;
    while (lastElement->next != NULL) {
        temp = lastElement;
        lastElement = lastElement->next;
    }
    free(lastElement);
    temp->next = NULL;
    return head;
}

/* Add a new element at the queue of the list */

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

/* Add a new element at the head of the list */

Node* addFirst(Node* head, int value) {
    Node* newNode = createNode(value);
    newNode->next = head;
    return newNode;
}

/* Concatenation */

Node* concatenateLists(Node* list1, Node* list2) { // add the elements of list2 at the end of list1
    if (list1 == NULL) return list2;
    Node* temp = list1;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = list2;
    return list1;
}

/* Apply a function to every element of the list */

int add3(int x) {
    return x + 3;
}

Node* mapList(Node* head, int (*func)(int)) {
    Node* temp = head;
    while (temp != NULL) {
        temp->data = func(temp->data);
        temp = temp->next;
    }
    return head;
}

/* Transform the linked list in a doubly linked list */

typedef struct doublyLinkedNode {
    int data;
    struct doublyLinkedNode* prev;
    struct doublyLinkedNode* next;
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
        temp = temp->next;
        newDoubleNode = createDoublyLinkedNode(temp);
        newDoubleNode->prev = doubleTemp;
        doubleTemp->next = newDoubleNode;
        doubleTemp = newDoubleNode;
    }
    return doubleHead;
}

/* Create a doubly linked cycle list out of a doubly linked list */

DoublyLinkedNode* makeItCycle(DoublyLinkedNode* head) {

    if (head == NULL) {
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

/* Create a doubly linked cycle list of n elements */

DoublyLinkedNode* createCycle(int n) {
    DoublyLinkedNode* head = createDoublyLinkedChain(createList(n));
    head = makeItCycle(head);
    return head;
}

/* Create a function to show an element of a doubly linked list */

void displayDoublyLinkedNode(DoublyLinkedNode* head) {
    printf("Adresse du maillon: %p, Valeur du maillon: %d\n", head, head->data);
}


void exo3() {
    printf("\nCreation de la liste a :\n");
    int sizeList = 8;
    Node* list_a = createList(sizeList);
    displayList(list_a);

    printf("\nCreation de la liste b :\n");
    sizeList = 4;
    Node* list_b = createList(sizeList);
    displayList(list_b);

    printf("\nConcatène la liste a et la liste b :\n");
    list_a = concatenateLists(list_a, list_b);
    displayList(list_a);

    sizeList = listLength(list_a);
    printf("\nLa nouvelle taille de la liste a est : %i\n", sizeList);

    printf("\nAfficher les adresses des éléments de la liste :\n");
    displayListWithAddress(list_a);

    printf("\nSupprimer les premier et dernier éléments de la liste :\n");
    list_a = removeFirst(list_a);
    list_a = removeLast(list_a);
    displayList(list_a);


    printf("\nAjouter les éléments 25 en tête de liste et 37 en queue :\n");
    list_a = addFirst(list_a, 25);
    list_a = addLast(list_a, 37);
    displayList(list_a);

    printf("\nAjouter 3 à tous les éléments de la liste :\n");
    list_a = mapList(list_a, add3);
    displayList(list_a);
    

    printf("\nTransformer la liste a en une double liste chaînée cyclique :\n");
    DoublyLinkedNode* doublyLinkedList_a = createDoublyLinkedChain(list_a);
    DoublyLinkedNode* cycle = makeItCycle(doublyLinkedList_a);
    for (int i = 0; i < sizeList + 3; i++){
        displayDoublyLinkedNode(cycle);
        cycle = cycle->next;
    }

    printf("\nCréer une double liste chaînée cyclique de n éléments :\n");
    DoublyLinkedNode* newCycle = createCycle(sizeList);
    for (int i = 0; i < sizeList + 3; i++){
        displayDoublyLinkedNode(newCycle);
        newCycle = newCycle->next;
    }
}

int main(void)
{
    exo1();
    exo2();
    exo3();
    return 0;
}