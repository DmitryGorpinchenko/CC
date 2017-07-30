#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} node;

node* new_node(int data);
node* push_front(node* head, int data);
node* push_back(node* head, int data);
node* find(node* head, int data);
node* delete(node* head, node* elem);
void free_list(node** head);
void print_list(node* head);

int main() {
    node* head = NULL, *np;
    puts("should display empty");
    print_list(head);
    
    //test push_front
    head = push_front(head, 10);
    head = push_front(head, 20);
    puts("should display 20 10");
    print_list(head);
    
    //test free list
    free_list(&head);
    puts("should display empty");
    print_list(head);
    
    //test push_back
    head = push_back(head, 10);
    head = push_back(head, 20);
    head = push_back(head, 30);
    puts("should display 10 20 30");
    print_list(head);
     
    //test find
    np = find(head, -20);
    puts("should display empty");
    print_list(np);
    
    np = find(head, 20);
    puts("should display 20 30");
    print_list(np);
        
    //test delete
    head = delete(head, np);
    puts("should display 10 30");
    print_list(head);
    
    np = find(head, 10);
    head = delete(head, np);
    puts("should display 30");
    print_list(head);
     
    //clean up
    free_list(&head);    
    return 0;
}

node* new_node(int data) {
    node* node_ptr = (node*) malloc(sizeof(node));
    if(node_ptr != NULL) {
        node_ptr->data = data;
        node_ptr->next = NULL;
    }
    return node_ptr;
}

node* push_front(node* head, int data) {
    node* node_ptr = new_node(data);
    if(node_ptr != NULL) {
        node_ptr->next = head;
        head = node_ptr;
    }
    return head;
}

node* push_back(node* head, int data) {
    node* node_ptr = new_node(data);
    if(node_ptr == NULL) {
        return head;
    }
    if(head == NULL) {
        return (head = node_ptr);
    }
    node* ptr = head;
    while(ptr->next != NULL) {
        ptr = ptr->next;
    }        
    ptr->next = node_ptr;
    return head;   
}

node* find(node* head, int data) {
    while(head != NULL && head->data != data) {
        head = head->next;
    }        
    return head;   
}

node* delete(node* head, node* elem) {
    if(elem == NULL) {
        return head;
    }
    if(elem == head) {
        node* tmp = head;
        head = tmp->next;
        free(tmp);
        return head;
    }
    node* ptr = head;
    while(ptr->next != elem) {
        ptr = ptr->next;
    }   
    node* tmp = ptr->next;
    ptr->next = tmp->next;
    free(tmp);
    return head;
}

void free_list(node** head) {
    node* ptr = *head;
    while(ptr != NULL) {
        node* tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }   
    *head = NULL;    
}

void print_list(node* head) {
    while(head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }    
    printf("\n");    
}

