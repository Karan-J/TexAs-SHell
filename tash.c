#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>
#include<stdbool.h>

//The one and only error message of the program.
#define ERROR_MSG "An error has occured\n"
#define DEFAULT_PATH "/bin"
char* gPath = NULL;

void parseAndExecute(char * iLineBuffer)
{
    if(NULL == iLineBuffer)
    {
        return;
    }

    char *tSpaceToken = NULL;
    char *tSaveSpaceToken = NULL;
    char *tAmpersandToken = NULL;
    char *tSaveAmpersandToken = NULL;
    const char tSpaces[] = " \t\r\n\v\f";
    const char tAmpersand[] = "&";

    for( tAmpersandToken=strtok_r(iLineBuffer,tAmpersand,&tSaveAmpersandToken) ; tAmpersandToken!=NULL ; tAmpersandToken=strtok_r(NULL,tAmpersand,&tSaveAmpersandToken) )
    {
        for(tSpaceToken=strtok_r(tAmpersandToken,tSpaces,&tSaveSpaceToken) ; tSpaceToken!=NULL ; tSpaceToken=strtok_r(NULL,tSpaces,&tSaveSpaceToken) ) 
        {
            // printf("%s ",tSpaceToken);
        }
        printf("\n");
    }
}

bool initBatch(char* iArgv)
{
    if(NULL == iArgv)
    {
        return false;
    }

    FILE *tFileHandler;
    char *tLineBuffer = NULL;
    size_t tLineBufferSize = 0;

    tFileHandler = fopen(iArgv,"r");

    if(NULL == tFileHandler)
    {
        return false;
    }
    
    while(true)
    {
        tLineBufferSize = getline(&tLineBuffer,&tLineBufferSize,tFileHandler);

        if(EOF == tLineBufferSize)
        {
            break;
        }

        parseAndExecute(tLineBuffer);
    }

    return true;
}

void modifyPath(char* iPath)
{
    if(NULL != gPath)
    {
        free(gPath);
        gPath = NULL;
    }

    gPath = (char *) malloc(100);

    if(NULL != gPath)
    {
        strncpy(gPath,iPath,100);
    }
    else
    {
        gPath = (char *) malloc(100);

        if(NULL != gPath)
        {
            strncpy(gPath,DEFAULT_PATH,100);
        }
    }
}

bool initTash(int iArgc, char** iArgv)
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
            return initBatch(iArgv[iArgc-1]);
        }
        default:
        {
            return false;
        }
    }

    return true;
}

//Main
int main(int iArgc, char** iArgv)
{
    modifyPath(DEFAULT_PATH);

    printf("PATH: %s\n",gPath);

    if(true != initTash(iArgc,iArgv))
    {
        return write(STDERR_FILENO,ERROR_MSG,strlen(ERROR_MSG));
    }

    return 0;
}