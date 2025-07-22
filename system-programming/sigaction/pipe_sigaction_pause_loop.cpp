#include<unistd.h>
#include<csignal>
#include<cstring>
#include<iostream>
#include<sys/wait.h>

int pipefd[2];
bool data_ready = false;

void signal_handler(int sig){
    std::cout<<"Signal Received: "<<sig<<std::endl;
    data_ready = true;
}

int main(){
    pipe(pipefd);

    pid_t pid = fork();

    if(pid==0){
        close(pipefd[1]);

        struct sigaction sa;

        sa.sa_handler = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;

        sigaction(SIGUSR1, &sa, NULL);

        std::cout<<"Child PID: "<<getpid()<<" Child waiting for Signa.... "<<std::endl;

        while(!data_ready) pause();

        char buffer[128];

        int n = read(pipefd[0], buffer, sizeof(buffer));
        buffer[n] = '\0';

        std::cout<<"Child received data: "<<buffer<<std::endl;

    
    }else{
        close(pipefd[0]);

        sleep(2);

        const char* msg = "This is message from Parent to Child!";

        write(pipefd[1], msg, strlen(msg));

        kill(pid, SIGUSR1);

        wait(NULL);
    }
    return 0;
}