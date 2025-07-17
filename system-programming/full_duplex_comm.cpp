#include <unistd.h>
#include <iostream>
#include <cstring>

int main()
{
    int pipefd1[2], pipefd2[2]; // pipe[0] = read end ;; pipe[1] = write end

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {

        // Child Section

        close(pipefd1[1]); // closes write end of pipe1 which parent will write only
        close(pipefd2[0]); // closes read end of pipe2 which parent will read only

        char buffer[128];

        int n = read(pipefd1[0], buffer, sizeof(buffer));

        buffer[n] = '\0';

        std::cout << "Child received(from parent msg): " << buffer << std::endl;

        const char *child_reply = "This reply messsage is from Child to Parent! Enjoy!";

        write(pipefd2[1], child_reply, strlen(child_reply));

        close(pipefd1[0]);
        close(pipefd2[1]);
    }
    else
    {

        // Parent section

        close(pipefd1[0]); // closes read end as parent will write to this pipe and child reads this end
        close(pipefd2[1]); // closes write end as parent will read the reply from child from pipe[0];

        const char *parent_msg = "This message is from parent to Child and will get the reply after this msg received by child!";

        write(pipefd1[1], parent_msg, strlen(parent_msg));

        char buffer[128];
        int n = read(pipefd2[0], buffer, sizeof(buffer));
        buffer[n] = '\0';

        std::cout<<"Parent received: "<<buffer<<std::endl;

        close(pipefd1[1]);
        close(pipefd2[0]);
    }
    return 0;
}