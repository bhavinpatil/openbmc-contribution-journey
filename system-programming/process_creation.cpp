#include <unistd.h>   // fork(), execvp()
#include <sys/wait.h> //wait, WEXITSTATUS
#include <iostream>   //cout, perror

void runChildProcess(char *args[])
{
    pid_t pid = fork();

    // fork creates new process : returns - 0 in the child
    // - PID of child in parent
    // -1 on failure.

    if (pid < 0)
    {
        perror("fork failed");
        _exit(1);
    }
    if (pid == 0)
    {
        // Child process block in if block
        execvp(args[0], args);

        // If exes fails
        perror("execvp failed");
        _exit(1);
    }
    else
    {
        // Parent process block in else block
        int status;
        wait(&status);

        std::cout << "Child process exited with Status: " << WEXITSTATUS(status) << std::endl;
    }
}

int main()
{
    char* args1[] = {(char*)"ls", (char*)"-l", NULL};
    // char* args2[] = {(char*)"ps", (char*)"-aux", (char*)"|", (char*)"grep", (char*)"creation", NULL};
    char* args2[] = {(char*)"ps", (char*)"-aux", NULL};
    char* args3[] = { (char*)"date", NULL };

    char* args4[] = { (char*)"echo", (char*)"Hello mCoreLogic", NULL };

    char* args5[] = { (char*)"fake_command", NULL };  // Will trigger perror


    
    runChildProcess(args1);
    runChildProcess(args2);
    runChildProcess(args3);
    runChildProcess(args4);
    runChildProcess(args5);
    
    return 0;
}