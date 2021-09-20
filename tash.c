#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>
#include<stdbool.h>

#define ERROR_MSG "An error has occured\n"

bool initTash(int iArgc, char** argv)
{
    switch(iArgc)
    {
        case 1:
        {
            printf("Interactive Mode\n");
            //TODO: Define initInteractive()
            break;
        }
        case 2:
        {
            printf("Batch Mode\n");
            //TODO: Define initBatch()
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    if(true != initTash(argc,argv))
    {
        return write(STDERR_FILENO,ERROR_MSG,strlen(ERROR_MSG));
    }

    return 0;
}