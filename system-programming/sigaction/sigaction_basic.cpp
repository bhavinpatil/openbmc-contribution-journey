#include<csignal>
#include<iostream>
#include<unistd.h>

void handle_signal(int sig){
    std::cout<<"Received Signal : "<<sig<<std::endl;
}

int main(){
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);

    std::cout<<"PID: "<<getpid()<<"- Waiting for SIGUSR1 signal...."<<std::endl;

    pause();

    std::cout<<"Signal Received! Exiting Process."<<std::endl;
    return 0;
}