#include<csignal>
#include<iostream>
#include<unistd.h>

void handle_signal(int sig, siginfo_t* info, void* context){
    std::cout<<"Signal received: "<<sig<<std::endl;
    std::cout<<"Signal Code: "<<info->si_code<<std::endl;
    std::cout<<"Signal PID: "<<info->si_pid<<std::endl;
    std::cout<<"Signal UID: "<<info->si_uid<<std::endl;
}

int main(){
    struct sigaction sa;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    std::cout<<"PID: "<< getpid()<<" Send SIGUSR1 signal..."<<std::endl;
    pause();
    pause();
    pause();
    std::cout<<"Exiting....!"<<std::endl;

    return 0;
}