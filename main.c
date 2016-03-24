#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#define MAX_WORD_SIZE 256

char* getNextWord(FILE* fd, struct hash_table_ *table)
{
    char* word = (char*)malloc(MAX_WORD_SIZE);
    char ch;
    int size=0;

    word[0]='\0';

    while( (ch=fgetc(fd)) != ' ' )
    {
        if(ch==EOF)
            break;
        if(size > MAX_WORD_SIZE-1)
            break;
        word[size]=ch;
        size++;
    }
    if(word[0]=='\0')
        return NULL;
    word[size]='\0';

    if(word!=NULL)
    {
        void *ret = hash_get(table, word);
        if(ret==NULL)
        {
            int *d;
            d = (int *)malloc(sizeof(int));
            *d=1;
            hash_insert(table, word, d);
        }
        else
        {
            int count = *((int *)ret);
            count=count+1;

            int *d;
            d = (int *)malloc(sizeof(int));
            *d=count;
            hash_insert(table, word, d);
        }
    }

    return word;
}

int main(int argc, char **argv)
{

    struct hash_table_ *table;
    int size = 500;

    table = create_hash_table(size);

    int n = -1;
    int startArg=1;
    if(argv[1][0]=='-')
    {
        startArg=2;
        n = -1*atoi(argv[1]);
    }

    int i;
    for(i=startArg;i<argc;i++)
    {
        FILE *fd;
        fd = fopen(argv[i],"r");


        char* str;
        while( (str=getNextWord(fd, table))!=NULL);

        fclose(fd);
    }

    print_hash_table(table, n);

    return 0;
}


