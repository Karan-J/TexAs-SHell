/**
 * @brief: tash.c
 * 
 * @paragraph: Description - This is an implementation of a user-made shell. We are calling it tash (TexAs-SHell). Feel free to play around with it. This is a project developed toward the requirements of Project-1 in the course CS 5348 : Operating Systems Concepts.
 * 
 * @authors: Shreyans Patel (SSP210009), Karan Jariwala (KHJ200000)
 */

//This order is very important for compilation. Add new libraries only in lib.h.
#include "lib.h"
#include "def.h"
#include "funct.c"

/**
 * @brief: main.
 * @details: The main function of tash. This is the file that is to be compiled and executed.
 * @param: iArgc (Input) - The number of input arguments passed while executing "./tash". "./tash" counts too.
 * @param: iArgv (Input) - Array of argument strings.
 * @return int.
 */
int main(int iArgc, char** iArgv)
{
    if(NULL != iArgv[0])    //Debugging Only
    {
        printf("Argument 1: %s\n",iArgv[0]);
    }

    if(NULL != iArgv[1])    //Debugging Only
    {
        printf("Argument 2: %s\n",iArgv[1]);
    }

    if(NULL != iArgv[2])    //Debugging Only
    {
        printf("Argument 3: %s\n",iArgv[2]);
    }

    modifyPath(DEFAULT_PATH);   //Set path variable to default "/bin"

    initTash(iArgc,iArgv);   //Initialize tash and derive operation mode.

    //Exit Shell
    return 0;
}