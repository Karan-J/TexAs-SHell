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

    // char **tCmdArr;

    for( tAmpersandToken=strtok_r(iLineBuffer,tAmpersand,&tSaveAmpersandToken) ; tAmpersandToken!=NULL ; tAmpersandToken=strtok_r(NULL,tAmpersand,&tSaveAmpersandToken) )
    {
        for(tSpaceToken=strtok_r(tAmpersandToken,tSpaces,&tSaveSpaceToken) ; tSpaceToken!=NULL ; tSpaceToken=strtok_r(NULL,tSpaces,&tSaveSpaceToken) ) 
        {
            
            printf("%s ",tSpaceToken);
        }
        printf("\n");
    }
}

void initBatch(char* iArgv)
{
    if(NULL == iArgv)
    {
        return;
    }

    FILE *tFileHandler;
    char *tLineBuffer = NULL;
    size_t tLineBufferSize = 0;

    tFileHandler = fopen(iArgv,"r");

    if(NULL == tFileHandler)
    {
        return;
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
}

//Initialize tash
void initTash(int iArgc, char** iArgv)
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
            initBatch(iArgv[iArgc-1]);
            break;
        }
        default:
        {
            break;
        }
    }
}

//Change path variable for the program
void modifyPath(char* iPath)
{
    if(NULL != gPath)
    {
        free(gPath);
        gPath = NULL;
    }

    gPath = (char *) malloc(DEFAULT_PATH_SIZE);

    if(NULL != gPath)
    {
        strncpy(gPath,iPath,DEFAULT_PATH_SIZE);
    }
    else
    {
        gPath = (char *) malloc(DEFAULT_PATH_SIZE);

        if(NULL != gPath)
        {
            strncpy(gPath,DEFAULT_PATH,DEFAULT_PATH_SIZE);
        }
    }
}

//tTemp in this function is used to avoid compilation errors since we need to use the return value of write().
void printErrorMsg()
{
    int tTemp = 0;
    tTemp = write(STDERR_FILENO,ERROR_MSG,strlen(ERROR_MSG));
    tTemp++;
}