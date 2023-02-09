#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100

typedef struct Node
{
    char word[MAXSIZE] ;
    struct Node * left;
    struct Node * right;
}
Node;

Node * newNode(char val [])
{
    Node * n = malloc(sizeof(Node));
    strcpy(n -> word, val);
    n -> left = NULL;
    n -> right = NULL;
    return n;
}

Node* insert(Node* node, char key [])
{
    if (node == NULL)
    {
        return newNode(key);
    }

    if (strcasecmp(node -> word, key) > 0)
    {
        node -> left = insert(node -> left, key);
    }

    else if (strcasecmp(node -> word, key) < 0)
    {
        node -> right = insert(node->right, key);
    }

    return node;
}

Node * search(Node * n, Node ** parent, char val [])
{
    if(n == NULL)
    {
        return NULL;
    }
    else if(strcasecmp(n -> word, val) == 0)
    {
        *parent = n;
        return n;
    }
    else if(strcasecmp(n -> word, val) > 0)
    {
        *parent = n;
        return search(n -> left, &*parent,  val);
    }
    else
    {
        *parent = n;
        return search(n -> right, &*parent, val);
    }
}
Node * load(char filename [])
{
    FILE * file = fopen(filename, "r");
    char temp [50];
    Node * root = NULL, * res=NULL;
    if(file != NULL)
    {
        fscanf(file, "%s", temp);
        root = newNode(temp);
        while(!feof(file))
        {
            fscanf(file, "%s", temp);
            res = insert(root, temp);
        }
        printf("Dictionary loaded successfully\n");
    }
    else
    {
        printf("File not found\n");
    }
    return root;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int height(Node * root)
{
    if(root == NULL)
    {
        return -1;
    }
    else
    {
        return 1 + max(height(root -> left), height(root -> right));
    }
}

int count(Node * root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + count(root -> left) + count(root -> right);
    }
}

Node * getRoot(Node * n, Node * parent)
{
    if(n == NULL)
    {
        return NULL;
    }
    else if(n -> left == parent)
    {
        return n;
    }
    else if(n -> right == parent)
    {
        return n;
    }
    else if(strcasecmp(n -> word, parent -> word) > 0)
    {
        return getRoot(n -> left, parent);
    }
    else if(strcasecmp(n -> word, parent -> word) < 0)
    {
        return getRoot(n -> right, parent);
    }
}

void suggestions(Node * root, Node * parent)
{
    Node * temp = NULL;
    Node * successor = NULL;
    Node * predecessor = NULL;
    temp = getRoot(root, parent);
    if(temp -> left == parent)
    {
        if(parent -> right == NULL)
        {
            successor = getRoot(root, parent);
        }
        else
        {
            while(parent -> right -> left != NULL)
            {
                parent -> right = parent -> right -> left;
            }
            successor = parent -> right;
        }
        if(parent -> left == NULL)
        {
            while(getRoot(root, temp) -> right != temp)
            {
                temp = getRoot(root, temp);
            }
            predecessor = getRoot(root, temp);
        }
        else
        {
            while(parent -> left -> right != NULL)
            {
                parent -> left = parent -> left -> right;
            }
            predecessor = parent -> left;
        }
    }
    else if(temp -> right == parent)
    {
        if(parent -> left == NULL)
        {
            predecessor = getRoot(root , parent);
        }
        else
        {
            while(parent -> left -> right != NULL)
            {
                parent -> left = parent -> left -> right;
            }
            predecessor = parent -> left;
        }
        if(parent -> right == NULL)
        {
            while(getRoot(root, temp) -> left != temp)
            {
                temp = getRoot(root, temp);
            }
            successor = getRoot(root, temp);
        }
        else
        {
             while(parent -> right -> left != NULL)
            {
                parent -> right = parent -> right -> left;
            }
            successor = parent -> right;
        }
    }
    printf("%s %s %s\n", parent -> word, successor -> word, predecessor -> word);
}

int main()
{
    char filename[100];
    Node * parent;
    printf("Enter the filename:\n");
    gets(filename);
    Node *root=load(filename);
    printf("..................................\n");
    int size=count(root);
    printf("Size = %d\n",size);
    printf("..................................\n");
    int h=height(root);
    printf("Height = %d\n",h);
    printf("..................................\n");
    char sentence [1000];
    printf("Enter a sentence\n");
    gets(sentence);
    char * token = strtok(sentence, " ");
    while(token != NULL)
    {
        if(search(root, &parent, token) != NULL)
        {
            printf("%s - correct\n", token);
            token = strtok(NULL, " ");
        }
        else if(search(root, &parent, token) == NULL)
        {
            printf("%s - Incorrect, Suggestions : ", token);
            suggestions(root, parent);
            token = strtok(NULL, " ");
        }
    }
    return 0;
}
