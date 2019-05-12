/****************************************************************
 * Name        :  Joe Binalinbing                               *
 * Class       :  CSC 415                                       *
 * Date        :     3/15/2019                                  *
 * Description :  Writting a simple bash shell program          *
 *                that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/

#include <string.h>
#include <unistd.h>  //Definition for fork() and execve()
#include <stdio.h>
#include <sys/stat.h>
#include <wait.h>
#include <stdlib.h>
#include <errno.h>    // Definition for "error handling"
#include <fcntl.h>

/* CANNOT BE CHANGED */
#define BUFFERSIZE 256
/* --------------------*/
#define PROMPT "myShell >> "
#define PROMPTSIZE sizeof(PROMPT)
#define ARGVMAX 64
#define PIPECNTMAX 10
#define MAX_CMD_LEN  128
#define HISTORY_COUNT 20

/* Declaration for strtok() */

/*
 *
 * Reference I used:
 * https://github.com/szholdiyarov/command-line-interpreter/blob/master/myshell.c
 *  https://www.geeksforgeeks.org/making-linux-shell-c/
 *  http://www.sarathlakshman.com/2012/09/24/implementation-overview-of-redirection-and-pipe-operators-in-shell
 * https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection
 * http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html
 *  https://github.com/jyu13/CSC415/blob/master/3.Shell/myshell.c
 *
 * */

int exitFlag = 1;
int inFlag = 0, outFlag1 = 0, outFlag2 = 0, pipeFlag = 0, backgroundFlag = 0, commandFlag = 0;

void startDisplay ()
{

    printf ("**************************************************************\n");
    printf ("*WELCOME TO MY SHELL                                         *\n");
    printf ("*USE AT YOUR OWN RISK                                        *\n");
    printf ("*TO USE MI-CHELLE:                                           *\n");
    printf ("*EXAMPLE: ls -1                                              *\n");
    printf ("*TO EXIT TYPE 'q'                                            *\n");
    printf ("**************************************************************\n");

}

/* Divide input line into tokens */
int parseTokens (char* input, char** arrayOfPar)
{
    int i, myargc = -1;
    for (i = 0; i<ARGVMAX; i++) {

        arrayOfPar[i] = strsep (&input, " ");

        printf ("Testing parsing tokens: The array string is %s\n", arrayOfPar[i]);

        myargc++;

        if (arrayOfPar[i]==NULL)
            break;

    }

    printf ("Testing number of args stored : n = %d\n", myargc);
    printf ("\n");

    return myargc;
}

void execute (char** command)
{

    pid_t pid;

    pid = fork ();

    if (pid==0) {

        if (execvp (command[0], command)==-1) {

            exit (0);

        }

    }

    else {

        if (backgroundFlag==0) {

            wait (NULL);

        }

    }

}

int processSymbols (char** myargv, int myargc, char** argv1, char** argv2)
{
    /**
     * TOO LAZY TO SEPARATE COMMANDS FROM SYMBOLS
     *
     *
     *
     *
     */

    int i, x, j, pipeCount = 1, flag;

    for (i = 0; i<myargc; i++) {
        /* Check if input is "q", if yes then exit shell */

        if (strcmp (myargv[i], "q")==0) {
            printf ("SYSTEM : Shell is exit\n");

            exitFlag = i;

        }

        else if (strcmp (myargv[0], "cd")==0) {
            if (myargv[0]!=NULL) { //only change directory if directory is specified
                chdir (myargv[1]);
            }
        }
            //check for pwd command
        else if (strcmp (myargv[0], "pwd")==0) {
            long size;
            char* cwd;
            size = pathconf (".", _PC_PATH_MAX);
            if ((cwd = (char*) malloc ((size_t) size))!=NULL) {
                getcwd (cwd, (size_t) size);
                printf ("Current working directory: %s\n", cwd);
            }
        }
        else if (strcmp (myargv[i], ">")==0) {
            printf ("SYSTEM : '>' is entered\n");
            printf (" Directs the output of a command into a file.\n");
            outFlag2 = i;
            flag = 2;
        }
        else if (strcmp (myargv[i], ">>")==0) {
            printf ("SYSTEM : '>>' is entered\n");
            printf (" new data are appended\n");
            outFlag1 = i;
            flag = 2;

        }
        else if (strcmp (myargv[i], "<")==0) {
            printf ("SYSTEM : '<' is entered\n");
            printf (" Gives input to a command\n");
            inFlag = i;
            flag = 2;
        }
        else if (strcmp (myargv[i], "|")==0) {

            //  pipeFlag = i;
            printf ("SYSTEM : '|' is entered\n");

            printf (" pipe is found \n");
            printf ("\n");

            pipeCount = pipeCount+1;

            //   printf("Testing for pipecount: is %d\n", pipeCount);

            //printf("Testing for i: is %d\n", i);

            //  int z3, z4, z5, z6, z7, z8, z9, z10;



            if (pipeCount==2) {
                flag = 3;
                for (x = 0; x<i; x++) {

                    argv1[x] = myargv[x];

                }

                //  printf ("Testing: x total is %d\n", x);

                int z = 0;

                for (x = i+1; x<myargc; x++) {
                    // int z = 0;

                    argv2[z] = myargv[x];

                    z++;
                    if (strcmp (myargv[x], "|")==0) {
                        argv2[z-1] = '\0';
                        break;
                    }

                    flag = 3;

                }////PIPE 1
            }
//            if (pipeCount==3) {
//                flag = 5;
//                int y = 0;
//                for (x = i+1; x<myargc; x++) {
//                    argv3[y] = myargv[x];
//                    y++;
//                    if (strcmp (myargv[x], "|")==0) {
//                        argv3[y-1] = '\0';
//                        break;
//                    }
//                }
//                int z = 0;
//                for (x = x+1; x<myargc; x++) {
//                    argv4[z] = myargv[x];
//                    z++;
//                    if (strcmp (myargv[x], "|")==0) {
//                        argv4[z-1] = '\0';
//                        break;
//                    }
//                }
//
//            }                                   //PIPE2



        }

    }

    return flag;

}

void executeRedirection (char** myargv, int myargc)
{
    int in, out, out2;
    int i;
    pid_t pid;

    if (myargv[1]!=NULL && outFlag1!=0) {
        out = open (myargv[outFlag1+1], O_WRONLY | O_CREAT | O_TRUNC, 00004 | 00040 | 00700);
        pid = fork ();
        if (pid==0) {
            dup2 (out, 1);
            for (i = outFlag1; i<myargc; i++) {
                myargv[i] = NULL;
            }
            if (execvp (myargv[0], myargv)==-1) {
                exit (0);
            }
        }
        close (out);
    }
    else if (myargv[1]!=NULL && outFlag2!=0) {
        out2 = open (myargv[outFlag2+1], O_WRONLY | O_CREAT | O_APPEND, 00004 | 00040 | 00700);
        pid = fork ();
        if (pid==0) {
            dup2 (out2, 1);
            for (i = outFlag2; i<myargc; i++) {
                myargv[i] = NULL;
            }
            if (execvp (myargv[0], myargv)==-1) {
                exit (0);
            }
        }
        else {
            if (backgroundFlag==0) {
                wait (NULL);
            }
        }
        close (out2);
    }
    else if (myargv[1]!=NULL && inFlag!=0) {
        in = open (myargv[inFlag+1], O_RDONLY);
        pid = fork ();
        if (pid==0) {
            dup2 (in, 0);
            for (i = inFlag; i<myargc; i++) {
                myargv[i] = NULL;
            }
            if (execvp (myargv[0], myargv)==-1) {
                exit (0);
            }
        }
        else {
            if (backgroundFlag==0) {
                wait (NULL);
            }
        }
    }

}

////single pipe
void executePipe (char** argv1, char** argv2)
{

    // 0 is read end, 1 is write end
    pid_t p1, p2;  ////create 2 process
    int pipefd[2];

    if (pipe (pipefd)<0) {
        perror ("\nPipe could not be initialized"); // Display error message

    }
    //process 1
    p1 = fork ();
    if (p1<0) {
        perror ("\nCould not fork");
    }

    else if (p1==0) {
        // Child 1 executing..
        // It only needs to write at the write end
        //dup lhs to stdout
        close (pipefd[1]);
        dup2 (pipefd[0], 0);

        //execute left hand side
        if (execvp (argv2[0], argv2)<0) {
            perror ("\nCould not execute command 1..");
            exit (errno);
        }
        //  exit(errno);;
    }

    //process 2
    p2 = fork ();

    if (p2<0) {
        perror ("\nCould not fork");
    }

        // Child 2 executing..
        // It only needs to read at the read end
    else if (p2==0) {
        //dup rhs to stdin
        close (pipefd[0]);
        dup2 (pipefd[1], 1);

        //execute right hand side
        if (execvp (argv1[0], argv1)<0) {
            perror ("\nCould not execute command 2..");
            exit (errno);
        }
        //exit(errno);
    }

    // parent executing, waiting for two children
    close (pipefd[0]);
    close (pipefd[1]);
    //wait for processes to finish

    wait (NULL);
    wait (NULL);

}

/*
 * loop over commands by sharing
 * pipes.
 */
static void pipeline (char*** cmd)
{
    int p[2];
    pid_t pid;
    int fd_in = 0;
    int i = 0;

    while (cmd[i]!=NULL) {
        pipe (p);
        if ((pid = fork ())==-1) {
            exit (1);
        }
        else if (pid==0) {
            dup2 (fd_in, 0);
            if (cmd[i+1]!=NULL)
                dup2 (p[1], 1);
            close (p[0]);
            execvp ((cmd)[i][0], cmd[i]);
            exit (2);
        }
        else {
            wait (NULL);
            close (p[1]);
            fd_in = p[0];
            i++;
        }
    }
}



// Function to print Current Directory.

void printDir ()
{
    char cwd[1024];
    getcwd (cwd, sizeof (cwd));
    //∗ myshell ~/ >>
    printf ("\n∗ myshell ~ %s >>", cwd);
}

int history (char* hist[], int current)
{
    int i = current;
    int hist_num = 1;

    do {
        if (hist[i]) {
            printf ("%4d  %s\n", hist_num, hist[i]);
            hist_num++;
        }

        i = (i+1)%HISTORY_COUNT;

    }
    while (i!=current);

    return 0;
}

int main (int argc, char** argv)
{

    char userInput[BUFFERSIZE+1];
    char* myargv[ARGVMAX+1] = {0};
    char* argv1[ARGVMAX+1] = {0};          ///single pipe 1
    char* argv2[ARGVMAX+1] = {0};            ///for single pipe 2
//    char* argv3[ARGVMAX+1] = {0};
//    char* argv4[ARGVMAX+1] = {0};
//    char* argv5[ARGVMAX+1] = {0};
//    char* argv6[ARGVMAX+1] = {0};
//    char* argv7[ARGVMAX+1] = {0};
//    char* argv8[ARGVMAX+1] = {0};
//    char* argv9[ARGVMAX+1] = {0};
//    char* argv10[ARGVMAX+1] = {0};

//    char** cmd[] = {argv1, argv2, argv3, NULL};
//    // Multiples pipes lol not the most ideal way

    char* hist[HISTORY_COUNT];
    int i, current = 0;
    int myargc, y, x; //counter for number of arguments

    startDisplay ();
    for (i = 0; i<HISTORY_COUNT; i++) {
        hist[i] = NULL;
    }
    while (exitFlag==1) {

        printDir ();
        // printf(PROMPT); // Display a user prompt

        fgets (userInput, sizeof (userInput), stdin); // Read the user input

        if (userInput[strlen (userInput)-1]=='\n') { //remove newline char
            userInput[strlen (userInput)-1] = '\0';
        }

        /* Check if input is empty */
        if (strcmp (userInput, " \n ")==0) {
            perror ("Please type in a command ");
            continue;
        }

        int k;
        for (k = 0; k<sizeof(userInput); k++) {

            if (userInput[k]=='&') {
                backgroundFlag = 1; //use 1 as "true"
                userInput[k-1] = '\0';

                //   printf("Testing parsing tokens: The array string is %s\n", userInput[k]);
                break; //continue without adding & to array
            }
        }

        free (hist[current]);
        hist[current] = strdup (userInput);
        current = (current+1)%HISTORY_COUNT;

      //  printf ("Testing for history: is :%s\n", hist[0]);

        int j = parseTokens (userInput, myargv); // Divide input into array of param

        int flag2 = processSymbols (myargv, j, argv1, argv2); //Process Special Symboools


//        for (i = 0; i<j; i++) {
//
//            printf ("Testing parsing argv1 The array string is %s\n", myargv[i]);
//        }
//
//        for (i = 0; i<j; i++) {
//
//            printf ("Testing parsing argv2 The array string is %s\n", argv2[i]);
//        }
//
//        for (i = 0; i<j; i++) {
//            printf ("Testing parsing argv3 The array string is %s\n", argv3[i]);
//
//        }
//
//        for (i = 0; i<j; i++) {
//
//            printf ("Testing parsing argv4 The array string is %s\n", argv4[i]);
//        }

//        printf ("Testing for pipe flags: is %d\n", pipeFlag);
//        printf ("Testing for command flags: is %d\n", commandFlag);
//        printf ("Testing for out flags: is %d\n", outFlag1);
//        printf ("Testing for outflags2: is %d\n", outFlag2);
//        printf ("Testing for inflags: is %d\n", inFlag);
//        printf ("Testing for process Flag: is %d\n", flag2);
//        printf ("\n");

        if (strcmp (userInput, "history")==0) {
            history (hist, current);
        }

        if (flag2==3 && commandFlag==0 && inFlag==0 && outFlag1==0 && outFlag2==0) {
            executePipe (argv1, argv2);
            // pipeline (cmd);
        }
        else if (flag2==2 && inFlag!=0 | outFlag1!=0 | outFlag2!=0 && pipeFlag==0 && commandFlag==0) {

            executeRedirection (myargv, j);
        }
//        else if (flag2==5) {
//
//            // pipeline (cmd);
//
//        }
        else {
            execute (myargv); // Call execvp()
        }

    }
    return 0;
}