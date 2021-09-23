/**
 * @brief: funct.h
 * 
 * @paragraph: Description - Support file of tash.c. It contains all the functions that will be used by tash while execution.
 * 
 * @authors: Shreyans Patel (SSP210009), Karan Jariwala (KHJ200000)
 */

/**
 * @brief: printErrorMsg.
 * @details: This function is used to print the one and only error message.
 * @return none.
 * @note: tTemp in this function is used to avoid compilation errors since we need to use the return value of write(). This is because of the compilation instructions being passed. Unused variables are considered errors in this case.
 */
void printErrorMsg(void)
{
    int tTemp = 0;
    tTemp = write(STDERR_FILENO,ERROR_MSG,strlen(ERROR_MSG));
    tTemp++;
}

/**
 * @brief: modifyPath.
 * @details: This is used to modify (set/update) the path variable.
 * @param: iPath (Input) - The path to set for the path variable.
 * @return none.
 */
void modifyPath(char* iPath)
{
    if(NULL == iPath)
    {
        printErrorMsg();
        return;
    }

    //This is required for overwriting the path variable.
    if(NULL != gPath)
    {
        free(gPath);
        gPath = NULL;
    }

    gPath = (char *) malloc(sizeof(iPath)+1);

    //If memory allocated successfully, assign new path to gPath. Otherwise, set gPath to default (/bin).
    if(NULL != gPath)
    {
        strcpy(gPath,iPath);
    }
    else
    {
        gPath = (char *) malloc(sizeof(DEFAULT_PATH)+1);

        if(NULL != gPath)
        {
            strcpy(gPath,DEFAULT_PATH);
        }
    }
}

/**
 * @brief: executeCmd.
 * @details: This command executes the command by forking.
 * @param: iPath (Input) - Path for the command source.
 * @param: iCmdArr (Input) - Command to execute.
 * @return none.
 */
void executeCmd(char *iPath,char **iCmdArr)
{
    int tRc = fork();

    if(tRc < 0)
    {
        printErrorMsg();
        exit(0);
    }

    else if(0 == tRc)
    {
        int tIndex = 0;
        while(NULL != iCmdArr[tIndex])
        {
            if (0 == strcmp(iCmdArr[tIndex],">"))
            {
                char *tOutputPath = NULL;
                tOutputPath = (char *) malloc(sizeof(iCmdArr[tIndex+2]) + 3);
                strcpy(tOutputPath,"./");
                strcat(tOutputPath,iCmdArr[tIndex+2]);
                close(STDOUT_FILENO);
                open(tOutputPath, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                iCmdArr[tIndex] = NULL;
                iCmdArr[tIndex+1] = NULL;
                break;
            }
            tIndex++;
        }
        
        if(-1 == execv(iPath,iCmdArr))
        {
            printErrorMsg();
            return;
        }
    }

    else
    {
        int tParent = wait(NULL);
        tParent++;
    }
}

/**
 * @brief: dispatchCmd.
 * @details: This command sends the command for execution after validation if command exists in path.
 * @param: iCmdArr (Input) - The data to dispatch for execution.
 * @param: iArrNullIdx (Input) - Index containing NULL in the array.
 * @return none.
 */
void dispatchCmd(char **iCmdArr,size_t iArrNullIdx)
{
    if(NULL == iCmdArr[0])
    {
        return;
    }

    //Built-in Command - exit.
    if(0 == strcmp("exit",iCmdArr[0]))
    {
        exit(0);
    }

    //Built-in Command - path.
    if(0 == strcmp("path",iCmdArr[0]))
    {
        char *tPath = NULL;

        tPath = (char *) malloc(sizeof(iCmdArr) + ((iArrNullIdx-1)*sizeof(char*)) + 1);

        if(NULL == tPath)
        {
            return;
        }

        int tIndex = 2;
        strcpy(tPath,iCmdArr[1]);
        strcat(tPath," ");

        while(NULL != iCmdArr[tIndex])
        {
            strcpy(tPath,iCmdArr[tIndex]);
            strcat(tPath," ");
            tIndex++;
        }

        modifyPath(tPath);
        printf("Path: %s\n",gPath);
        return;
    }
    
    ////Built-in Command - cd.
    if(0 == strcmp("cd",iCmdArr[0]))
    {
        if(-1 == chdir(iCmdArr[1]))
        {
            printErrorMsg();
        }
        return;
    }

    int tIsCmdFound = -1;   //To check if the path variable has the command user is trying to execute.
    char *tFinalPath = NULL;

    char *tSpaceToken = NULL;
    const char tSpaces[] = " \t\r\n\v\f";   //Delimiters

    //Tokenization for Spaces.
    for(tSpaceToken = strtok(gPath, tSpaces); tSpaceToken != NULL; tSpaceToken = strtok(NULL, tSpaces))
    {
        tFinalPath = (char *) malloc(sizeof(tSpaceToken) + sizeof(iCmdArr[0]) + 2);  //+2 for '/' path insertion and NULL.
        strcpy(tFinalPath,tSpaceToken);
        strcat(tFinalPath,"/");
        strcat(tFinalPath,iCmdArr[0]);

        tIsCmdFound = access(tFinalPath, X_OK);

        printf("%s\n",tFinalPath);

        //If command is present at the given path, execute it.
        if(0 == tIsCmdFound)
        {
            executeCmd(tFinalPath,iCmdArr);
            break;
        }
    }
    
    //Command not found in the path. Show error.
    if(-1 == tIsCmdFound)
    {
        printErrorMsg();
    }
}

/**
 * @brief: parseAndDispatch.
 * @details: This command parses the data received to a proper format to dispatch for execution.
 * @param: iLineBuffer (Input) - The data to parse (Commands in improper format).
 * @param: iSize (Input) - Size of characters in the data. Used for dynamic array memory allocation.
 * @return none.
 */
void parseAndDispatch(char * iLineBuffer, size_t iSize)
{
    //Validation before using the data.
    if((NULL == iLineBuffer) || (0 >= iSize))
    {
        printErrorMsg();
        return;
    }

    char *tCmdArr[iSize];
    size_t tCmdArrIdx = 0;

    char *tSpaceToken = NULL;
    char *tSaveSpaceToken = NULL;
    char *tAmpersandToken = NULL;
    char *tSaveAmpersandToken = NULL;
    const char tSpaces[] = " \t\r\n\v\f";   //Delimiters
    const char tAmpersand[] = "&";          //Delimiters

    //Tokenization for &.
    for(tAmpersandToken = strtok_r(iLineBuffer, tAmpersand, &tSaveAmpersandToken); 
        tAmpersandToken != NULL; 
        tAmpersandToken = strtok_r(NULL, tAmpersand, &tSaveAmpersandToken))
    {
        //Tokenization for whitspaces.
        for(tSpaceToken = strtok_r(tAmpersandToken, tSpaces, &tSaveSpaceToken); 
            tSpaceToken != NULL; 
            tSpaceToken=strtok_r(NULL, tSpaces, &tSaveSpaceToken)) 
        {
            tCmdArr[tCmdArrIdx] = strdup(tSpaceToken);
            tCmdArrIdx++;
        }

        tCmdArr[tCmdArrIdx] = NULL;

        dispatchCmd(tCmdArr,tCmdArrIdx);

        tCmdArrIdx = 0;
    }
}

/**
 * @brief: initBatch.
 * @details: Initialize batch mode of tash.
 * @param: iArgv (Input) - Array of argument strings.
 * @return none.
 */
void initBatch(char* iArgv)
{
    //Validation before accessing. For safety.
    if(NULL == iArgv)
    {
        printErrorMsg();
        return;
    }

    FILE *tFileHandler = NULL;
    char *tLineBuffer = NULL;
    size_t tLineBufferSize = 0;

    //Open the file.
    tFileHandler = fopen(iArgv,"r");

    //Opening the file failed.
    if(NULL == tFileHandler)
    {
        printErrorMsg();
        return;
    }
    
    //Read file line by line until end of file.
    while(true)
    {
        tLineBufferSize = getline(&tLineBuffer,&tLineBufferSize,tFileHandler);

        //If end of file is reached or if read failed.
        if(EOF == tLineBufferSize)
        {
            if(tLineBuffer != NULL)
            {
                free(tLineBuffer);
                tLineBuffer = NULL;
            }
            break;
        }

        //Send data for processing.
        parseAndDispatch(tLineBuffer,tLineBufferSize);
    }

    //Close the file.
    fclose(tFileHandler);
}

void initInteractive(void)
{	
	char *tLineBuffer = NULL;
	size_t tLineBufferSize = 0;

	printf("tash> ");
	
	while ((tLineBufferSize = getline(&tLineBuffer, &tLineBufferSize, stdin) != -1))
    {
		parseAndDispatch(tLineBuffer,tLineBufferSize);
        printf("tash> ");
    }		
}

/**
 * @brief: initTash.
 * @details: Initialization of tash program. It decides the mode of runtime i.e Interactive or Batch Mode.
 * @param: iArgc (Input) - The number of input arguments passed while executing "./tash". "./tash" counts too.
 * @param: iArgv (Input) - Array of arguments.
 * @return int.
 */
void initTash(int iArgc, char** iArgv)
{
    // printf("Debug ---------> 1\n");
    if(1 == iArgc)
    {
        initInteractive();  //Start tash in interactive mode.
    }
    else if(2 == iArgc)
    {
        initBatch(iArgv[iArgc-1]);  //Start tash in batch mode.
    }
    else
    {
        printErrorMsg();    //Invalid mode, show error. This is for more than one argument to ""./tash".
    }
}