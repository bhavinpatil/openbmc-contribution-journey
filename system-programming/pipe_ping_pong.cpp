#include <unistd.h>
#include <cstring>
#include <iostream>

int main()
{
    int parentToChild[2], childToParent[2];

    pipe(parentToChild);
    pipe(childToParent);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        // Child

        close(parentToChild[1]);
        close(childToParent[0]);

        char buffer[128];

        for (int i = 0; i < 3; i++)
        {
            int n = read(parentToChild[0], buffer, sizeof(buffer));

            buffer[n] = '\0';

            std::cout << "Child : " << buffer << std::endl;

            const char *reply = "Pong from child";

            write(childToParent[1], reply, strlen(reply));
        }

        close(parentToChild[0]);
        close(childToParent[1]);
    }
    else
    {
        close(parentToChild[0]);
        close(childToParent[1]);

        char buffer[128];
        for(int i=0; i<3; i++){
            const char* msg = "Ping from parent";
            write(parentToChild[1], msg, strlen(msg));

            int n = read(childToParent[0], buffer, sizeof(buffer));
            buffer[n] = '\0';

            std::cout<<"Parent: "<<buffer<<std::endl;
        }
        close(parentToChild[1]);
        close(childToParent[0]);
    }

    return 1;
}