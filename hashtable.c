#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORD_SIZE 256
typedef struct node_ {
	char *key;
	void *value;
	struct node_ * next;
} node;

typedef struct hash_table_ {
    int size;
    struct node_ **table;
} hash_table;


struct keyValPair {
    char key[MAX_WORD_SIZE];
    int val;
};

int numElems;
int numCollisions;

hash_table *create_hash_table(int size)
{
    hash_table *t;

    t = malloc(sizeof(hash_table));
    t->table = malloc(sizeof(node *)*size);

    int i;
    for(i=0;i<size;i++)
    {
        t->table[i] = NULL;
    }

    numElems = 0;
    numCollisions = 0;

    t->size = size;
    return t;
}

int hash(hash_table *t, char *str)
{
    int val=0;
    while(*str!='\0')
    {
        val = *str+(val<<5)-val;
        str++;
    }
    val = val%t->size;
    if(val<0)
        val=val*-1;
    return val;
}

// simply inserts new values BUT doesn't resize the has in case of more collisions
// this is basically used by other function for remapping elements
// collisions aren't counted here
void basic_hash_insert(hash_table *t, char *key, void *val)
{
    int idx = hash(t, key);

    node *n = t->table[idx];
    node *prev = NULL;

    while( n!=NULL && n->key != NULL && strcmp(key, n->key) > 0 )
    {
        prev = n;
        n = n->next;
    }

    if( n!=NULL && n->key != NULL && strcmp(key, n->key)==0 )
    {
        free(n->value);
        n->value = val;
    }
    else
    {
        node *newnode = malloc(sizeof(node));
        newnode->key = strdup(key);
        newnode->value = val;
        newnode->next = NULL;

        if(n==t->table[idx])
        {
            newnode->next = n;
            t->table[idx]=newnode;
        }
        else if(n==NULL)
        {
            prev->next = newnode;
        }
        else
        {
            newnode->next = n;
            prev->next = newnode;
        }

    }
}

void *hash_get(hash_table *t, char *key)
{
    int idx = hash(t, key);
    node * cur = t->table[idx];
    while(cur!=NULL && cur->key!=NULL && strcmp(key, cur->key) > 0)
    {
        cur=cur->next;
    }


    if(cur==NULL || cur->key==NULL || strcmp(key, cur->key) != 0)
        return NULL;
    else
        return cur->value;
}

void hash_insert(hash_table *t, char *key, void *val)
{
    int idx = hash(t, key);

    node *n = t->table[idx];
    node *prev = NULL;

    int cols=0;

    while( n!=NULL && n->key != NULL && strcmp(key, n->key) > 0 )
    {
        prev = n;
        n = n->next;
        cols++;
    }

    if( n!=NULL && n->key != NULL && strcmp(key, n->key)==0 )
    {
        free(n->value);
        n->value = val;
    }
    else
    {
        numElems++;
        if(cols>0)
            numCollisions++;

        node *newnode = malloc(sizeof(node));
        newnode->key = strdup(key);
        newnode->value = val;
        newnode->next = NULL;

        if(n==t->table[idx])
        {
            newnode->next = n;
            t->table[idx]=newnode;
        }
        else if(n==NULL)
        {
            prev->next = newnode;
        }
        else
        {
            newnode->next = n;
            prev->next = newnode;
        }

    }

    // RESIZING OF HASH TABLE IF MORE COLLISIONS START HAPPENING
    double ratio = ((double)numCollisions)/((double)numElems);
    if( ratio > 1.5 )
    {
        int curSize = t->size;
        hash_table * new_table = create_hash_table(curSize*3);

        int i;
        struct node_* head = NULL;
        for(i=0;i<t->size;i++)
        {
            head = (struct node_ *)t->table[i];
            while(head!=NULL)
            {
                void *ret = hash_get(t, head->key);

                // inserting elements into new bigger hash table
                basic_hash_insert(new_table,head->key, *((int *)ret));

                head=head->next;
            }
        }

        t = new_table; // pointing current table pointer to new bigger table

    }
}



int compareFunc (const void * a, const void * b)
{
    struct keyValPair * p1 = (struct keyValPair *) a;
    struct keyValPair * p2 = (struct keyValPair *) b;
    return -1*( p1->val - p2->val );
}

void print_hash_table(hash_table *t, int n)
{
    int i;
    struct node_* head = NULL;
    int numElems = 0;
    for(i=0;i<t->size;i++)
    {
        head = (struct node_ *)t->table[i];
        while(head!=NULL)
        {
            void *ret = hash_get(t, head->key);
            //printf("%10d %s\n",*((int*)ret),head->key);
            numElems++;
            head=head->next;
        }
    }


    struct keyValPair pairs[numElems];

    head = NULL;
    int ct=0;
    for(i=0;i<t->size;i++)
    {
        head = (struct node_ *)t->table[i];
        while(head!=NULL)
        {
            void *ret = hash_get(t, head->key);
            pairs[ct].val = *((int*)ret);

            // copying the contents of node key to pairs array
            char *st = head->key;
            int j;
            for(j=0;j<strlen(head->key);j++)
            {
                pairs[ct].key[j] = st[j];
            }
            pairs[ct].key[j] = '\0';

            ct++;
            head=head->next;
        }
    }

    qsort(pairs, numElems, sizeof(struct keyValPair),compareFunc);

    int limit = numElems;
    if(n>0)
        limit = n;
    for(i=0;i<limit;i++)
        printf("%10d %s\n",pairs[i].val,pairs[i].key);

}


