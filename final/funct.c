/**
 * @brief: funct.c
 * 
 * @paragraph: Description - Support file of tash.c. It contains all the functions that will be used by tash while execution.
 * 
 * @authors: Shreyans Patel (SSP210009), Karan Jariwala (KHJ200000)
 */

#include"funct.h"

/**
 * @brief: modifyPath.
 * @details: This is used to modify (set/update) the path variable.
 * @param: iPath (Input) - The paths to set for the path variable.
 * @return none.
 */
void modifyPath(char* iPath)
{
    if(NULL == iPath)
    {
        printErrorMsg();
        exit(0);
    }

    //This is required for overwriting the path variable.
    if(NULL != gPath)
    {
        free(gPath);
        gPath = NULL;
    }

    gPath = (char *) malloc(sizeof(iPath)*sizeof(char *));

    if(NULL == gPath)
    {
        printErrorMsg();
        exit(0);
    }

    strcpy(gPath,iPath);
}

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
 * @brief: initTash.
 * @details: Initialization of tash program. It decides the mode of runtime i.e Interactive or Batch Mode.
 * @param: iArgc (Input) - The number of input arguments passed while executing "./tash". "./tash" counts too.
 * @param: iArgv (Input) - Array of arguments.
 * @return int.
 */
void initTash(int iArgc, char** iArgv)
{
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
        exit(1);
    }
}

/**
 * @brief: initInteractive.
 * @details: Initialize interactive mode of tash.
 * @return none.
 */
void initInteractive(void)
{	
	char *tLineBuffer = NULL;
	size_t tLineBufferSize = 0;

	printf("tash> ");
	
	while ((tLineBufferSize = getline(&tLineBuffer, &tLineBufferSize, stdin) != -1))
    {
		parseAndDispatch(tLineBuffer);
        printf("tash> ");
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
    while(1)
    {
        tLineBufferSize = getline(&tLineBuffer,&tLineBufferSize,tFileHandler);

        //If end of file is reached or if read failed.
        if(EOF == tLineBufferSize)
        {
            break;
        }

        //Send data for processing.
        parseAndDispatch(tLineBuffer);
    }

    //Close the file.
    fclose(tFileHandler);
}

/**
 * @brief: parseAndDispatch.
 * @details: This command parses the data received to a proper format to dispatch for execution.
 * @param: iLineBuffer (Input) - The data to parse (Commands in improper format).
 * @return none.
 */
void parseAndDispatch(char * iLineBuffer)
{
    //Validation before using the data.
    if((NULL == iLineBuffer))
    {
        printErrorMsg();
        return;
    }

    const char *tCmdArr[DEFAULT_CMD_BUF_SIZE];
    size_t tCmdArrIdx = 0;

    //Tokenization process related variables.
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

        dispatchCmd(tCmdArr);

        tCmdArrIdx = 0;
    }
}

/**
 * @brief: dispatchCmd.
 * @details: This command sends the command for execution after validation if command exists in path.
 * @param: iCmdArr (Input) - The data to dispatch for execution.
 * @return none.
 */
void dispatchCmd(const char **iCmdArr)
{
    if(NULL == iCmdArr[0])
    {
        return;
    }

    //Built-in Command - exit.
    if(0 == strcmp("exit",iCmdArr[0]))
    {
        if(NULL != iCmdArr[1])
        {
            printErrorMsg();
            return;
        }

        exit(0);
    }

    //Built-in Command - path.
    if(0 == strcmp("path",iCmdArr[0]))
    {
        char *tPath = NULL;

        tPath = prepareSingleStrPath(iCmdArr);

        if(NULL == tPath)
        {
            return;
        }

        printf("Path: %s\n",tPath);
        printf("Path: %s\n",gPath);
        modifyPath(tPath);
        printf("Path: %s\n",gPath);
        return;
    }
    
    ////Built-in Command - cd.
    if(0 == strcmp("cd",iCmdArr[0]))
    {
        if((NULL != iCmdArr[2]) || (-1 == chdir(iCmdArr[1])))
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

        //If command is present at the given path, execute it.
        if(0 == tIsCmdFound)
        {
            executeCmd(tFinalPath,(char**)iCmdArr);
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
 * @brief: executeCmd.
 * @details: This command executes the command by forking.
 * @param: iPath (Input) - Path for the command source.
 * @param: iCmdArr (Input) - Command to execute.
 * @return none.
 */
void executeCmd(char *iPath,char **iCmdArr)
{
    pid_t tPid, tWaitPid;
    int tStatus;

    tPid = fork();

    //Forking failed, give error and return.
    if(tPid < 0)
    {
        printErrorMsg();
        exit(0);
    }

    //Child Loop.
    else if(0 == tPid)
    {
        int tIndex = 0;
        int tRedirectionCnt = 0;
        int tRedirectionFileNameIdx = 0;

        //Check if output is to be redirected.
        while(NULL != iCmdArr[tIndex])
        {
            //If two or more ">" in an argument, it is an error.
            if(NULL != strstr(iCmdArr[tIndex],">>"))
            {
                printErrorMsg();
                return;
            }

            //If one ">" is found, redirection is possible, do the needed validations.
            if (0 == strcmp(iCmdArr[tIndex],">"))
            {
                tRedirectionCnt++;
                
                tRedirectionFileNameIdx = tIndex + 1;

                if((NULL == iCmdArr[tIndex + 1]) || (NULL != iCmdArr[tIndex + 2]))
                {
                    printErrorMsg();
                    return;
                }
            }
            tIndex++;
        }
        
        //More than one redirection symbol used.
        if(1 < tRedirectionCnt)
        {
            printErrorMsg();
            return;
        }

        //Valid redirection, prepare the output file name from next argument.
        else if(1 == tRedirectionCnt)
        {
            char *tOutputPath = NULL;
            tOutputPath = strdup(iCmdArr[tRedirectionFileNameIdx]);
            //  (char *) malloc(sizeof(iCmdArr[tIndex+2])+1);
            // strcpy(tOutputPath,iCmdArr[tIndex+2]);
            close(STDOUT_FILENO);
            open(tOutputPath, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            iCmdArr[tRedirectionFileNameIdx-1] = NULL;
            iCmdArr[tRedirectionFileNameIdx] = NULL;
        }
        
        //Execv failed.
        if(-1 == execv(iPath,iCmdArr))
        {
            printErrorMsg();
            return;
        }
    }

    //Parent Process.
    else
    {
        do
        {
            tWaitPid = waitpid(tPid, &tStatus, WUNTRACED);
        } 
        while (!WIFEXITED(tStatus) && !WIFSIGNALED(tStatus));

        tWaitPid++;
    }
}

char * prepareSingleStrPath(const char** strings)
{
    int i = 1;              /* Loop index               */
    int count = 0;          /* Count of input strings   */
    char * result = NULL;   /* Result string            */
    int totalLength = 0;    /* Length of result string  */


    /* Check special case of NULL input pointer. */
    if (strings == NULL)
    {
        return NULL;
    }

    /* 
     * Iterate through the input string array,
     * calculating total required length for destination string.
     * Get the total string count, too.
     */
    while (strings[i] != NULL)
    {
        totalLength += (strlen(strings[i])+1);
        i++;
    }
    count = i;
    totalLength++;  /* Consider NUL terminator. */

    /*
     * Allocate memory for the destination string.
     */
    result = malloc(sizeof(char) * totalLength);
    if (result == NULL) 
    {
        /* Memory allocation failed. */
        return NULL;
    }

    /*
     * Concatenate the input strings.
     */
    for (i = 1; i < count; i++) 
    {
        strcat(result, strings[i]);
        strcat(result, " ");
    }

    return result;
}