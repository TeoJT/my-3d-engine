#include <stdio.h>


typedef struct struct_node {
    struct struct_node* prev;
    void* object;
    struct struct_node* next;
} Node;


typedef struct struct_linkedlist {
    Node* headNode;
    Node* tailNode;
    int size;
} LinkedList;

void addAtHead(LinkedList* list, void* object) {
    Node* newNode = malloc(sizeof(Node));
    newNode->object = object;
    newNode->prev = NULL;
    newNode->next = list->headNode;
    list->headNode = newNode;
    list->size++;
}

void insertAtIndex(LinkedList* list, int index, void* object) {
    Node* newNode = malloc(sizeof(Node));
    newNode->object = object;
    // If list is empty, add at head
    if (list->headNode == NULL) {
        newNode->prev = NULL;
        newNode->next = NULL;
        list->headNode = newNode;
        list->tailNode = newNode;
        list->size++;
        return;
    }

    Node* currentNode = list->headNode;
    for (int i = 0; i < index; i++) {
        currentNode = currentNode->next;
    }
    newNode->prev = currentNode->prev;
    newNode->next = currentNode;
    currentNode->prev = newNode;
    list->size++;
}

void addAtTail(LinkedList* list, void* object) {
    Node* n = malloc(sizeof(Node));
    n->object = object;

    n->next = NULL;


    if (list->size == 0) {
        list->headNode = n;
    }
    else {
        n->prev = list->tailNode;
        list->tailNode->next = n;
    }
    list->tailNode = n;

    list->size++;
}


