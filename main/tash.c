//This order is very important for compilation. Add new libraries only in lib.h.
#include "lib.h"
#include "def.h"
#include "funct.h"

int main(int iArgc, char** iArgv)
{
    modifyPath(DEFAULT_PATH);   //Set path variable to default "/bin"

    printf("PATH: %s\n",gPath);

    initTash(iArgc,iArgv);   //Initialize tash

    return 1;
}