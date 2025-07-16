#include<unistd.h> // pipe, fork, read, write, close
#include<iostream> //cout
#include<cstring> //strlen

int main(){
    int pipefd[2]; //pipefd[0] : read end ;; pipefd[1]:write end

    if(pipe(pipefd)==-1){
        perror("pipe");
        return 1;
    }


    pid_t pid = fork();

    if(pid<0){
        perror("fork");
        return 1;
    }

    if(pid==0){
        close(pipefd[1]); //closes write end as child will only read the data;

        char buffer[128];
        int n = read(pipefd[0], buffer, sizeof(buffer));

        buffer[n] = '\0';

        std::cout<<"Child received(read from pipefd[0]): "<<buffer<<std::endl;
        close(pipefd[0]);
    }else{
        close(pipefd[0]); //closes rea end as parent will only write the data;

        const char* msg = "This is msg from Parent!";

        write(pipefd[1], msg, strlen(msg));

        close(pipefd[1]);
    }

    return 1;
}