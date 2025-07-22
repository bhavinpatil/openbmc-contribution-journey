#include<unistd.h>
#include<csignal>
#include<iostream>
#include<cstring>
#include<sys/wait.h>

int pipefd[2];


void sig_handl(int sig){
    std::cout<<"Signal received: "<<sig<<std::endl;
    char buffer[128];
    int n = read(pipefd[0], buffer, sizeof(buffer));
    buffer[n] = '\0';
    std::cout<<"Child received message: "<<buffer<<std::endl;

}

int main(){

    pipe(pipefd); //pipefd[0] - read end ;; pipefd[1] - write end.

    pid_t pid = fork(); //creating or forking child process from main process

    if(pid==0){
        //Child Process section

        close(pipefd[1]);// it will close the write end and keep the read end open to read message from parents;

        struct sigaction sa;
        sa.sa_handler = sig_handl;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        std::cout<<"Child PID: "<<getpid()<<" Waiting for SIGUSR1...."<<std::endl;
        pause(); //pausing the child process till it gets the siganl from another terminal.
    }else{
        close(pipefd[0]); //closes the read end as it will write to the child process by keeping the write end open

        sleep(1); // keeping delay to ensure the child is ready to get the message or receive the signal

        const char* msg = "This is parent message for Child!, Hi, How are you?";
        write(pipefd[1], msg, strlen(msg));
        kill(pid, SIGUSR1);
        

        wait(NULL);


    }
    return 0;

}
