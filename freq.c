#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
    int linesToPrint = -1;
    int startArg=1;
    if(argv[1][0]=='-')
    {
        startArg=2;
        linesToPrint = -1*atoi(argv[1]);
    }

    int i;
    int ct=0;
    for(i=startArg;i<argc;i++)
    {
        FILE *fd;
        fd = fopen(argv[i],"r");
        char ch;
        while( ( ch = fgetc(fd) ) != EOF )
        {
            if(ct==linesToPrint)
                break;
            if(ch=='\n')
            {
                ct++;
            }
            printf("%c",ch);
        }
        fclose(fd);
        if(ct==linesToPrint)
            break;
        ct++; // one file has ended, so the last line is complete and hence needs to be counted
    }
}
