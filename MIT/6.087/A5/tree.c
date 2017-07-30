#include <stdio.h>
#include <stdlib.h>

typedef struct tnode {
    int data;
    struct tnode* lp;
    struct tnode* rp;
} tnode; 

tnode* talloc(int data);
tnode* addnode(tnode* root, int data);
void preorder(tnode* root);
void inorder(tnode* root);
int deltree(tnode* root);

int main() {
    tnode* root = NULL;
    int count = 0;
    root = addnode(root, 3);
    root = addnode(root, 1);
    root = addnode(root, 0);
    root = addnode(root, 2);
    root = addnode(root, 8);
    root = addnode(root, 6);
    root = addnode(root, 5);
    root = addnode(root, 9);
    
    //test preorder
    puts("should print 3, 1, 0, 2, 8, 6, 5, 9");
    preorder(root); puts("");

    //test inorder
    puts("should print 0, 1, 2, 3, 5, 6, 8, 9");
    inorder(root); puts("");
    
    //test deltree
    count = deltree(root); root = NULL;
    puts("should print 8 nodes deleted");
    printf("%d nodes deleted\n", count);
    
    return 0;
}

tnode* talloc(int data) {
    tnode* tp = (tnode*) malloc(sizeof(tnode));
    if(tp != NULL) {
        tp->data = data;
        tp->lp = NULL;
        tp->rp = NULL;
    }
    return tp;
}

tnode* addnode(tnode* root, int data) { 
    if(root == NULL) {
        return (root = talloc(data)); 
    }
    if(data < root->data) {
        root->lp = addnode(root->lp, data);
    } else {
        root->rp = addnode(root->rp, data);
    }
    return root;
}

void preorder(tnode* root) {
    if(root == NULL) {
        return;
    }    
    printf("%d ", root->data);
    preorder(root->lp);
    preorder(root->rp);
}

void inorder(tnode* root) {
    if(root == NULL) {
        return;
    }
    inorder(root->lp);    
    printf("%d ", root->data);
    inorder(root->rp);
}

int deltree(tnode* root) {
    if(root == NULL) {
        return 0;
    }
    int count = 0;
    count += deltree(root->lp);    
    count += deltree(root->rp);
    free(root);
    return ++count;
}
