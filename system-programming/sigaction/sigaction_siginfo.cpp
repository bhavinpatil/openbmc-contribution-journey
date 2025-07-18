#include<csignal>
#include<unistd.h>
#include<iostream>

void handle_signal(int sig, siginfo_t* info, void* context){
    std::cout<<"Received Signal: "<<sig<<std::endl;
    std::cout<<"Signal send by PID: "<<info->si_pid<<std::endl;
}

int main(){
    struct sigaction sa;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO; //enables extended info,

    sigaction(SIGUSR1, &sa, NULL);

    std::cout<<"PID:" <<getpid()<<"- waiting for SIGUSR1 signal...."<<std::endl;
    pause();

    std::cout<<"SIGUSR1 received!, exiting."<<std::endl;

    return 0;
}