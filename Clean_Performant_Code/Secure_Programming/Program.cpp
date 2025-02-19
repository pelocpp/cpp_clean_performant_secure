// ===========================================================================
// Program.cpp // Secure Programming
// ===========================================================================

extern void secure_programming_issues();
extern void secure_programming_more_issues();
extern void secure_programming_advices();
extern void secure_programming_tools();


// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 
#include <cstring>
#include <cstdlib>

void xxx1(int argc, char* argv[])
{
    char cmdLine[4096];
    cmdLine[0] = '\0';

    for (int i = 1; i < argc; ++i) {
        strcat(cmdLine, argv[i]);
        strcat(cmdLine, " ");
    }

}



// WEITER file:///C:/Users/peter/Downloads/df281b9bb8aa5c4377567454bb839676_MIT6_S096IAP14_Lecture3S.pdf

// MIT buflen = 1 am Anfang testen


void xxx2(int argc, char* argv[])
{
    size_t bufsize = 0;
    size_t buflen = 0;

    char* cmdLine = NULL;

    for (int i = 0; i < argc; ++i) {

        const size_t len = strlen(argv[i]);

        if (bufsize - len <= len) {

            bufsize = (bufsize + len) * 2;

            cmdLine = (char*)realloc(cmdLine, bufsize);

            if (cmdLine == NULL) {
                exit(-1);
            }
        }

    }

}

int main(int argc, char* argv[])
{
    xxx1(argc, argv);

   // secure_programming_issues();
    //secure_programming_more_issues();
    //secure_programming_advices();
    //secure_programming_tools();
    
    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
