// this program is example of how sa.sa_mask temporarily block certain signals while another signal is being handled.

#include<csignal>
#include<unistd.h>
#include<iostream>

void handle_sigusr1(int sig){
    std::cout<<"Signal received: "<<sig<<std::endl;
    std::cout<<"Handling SIGUSR1. Press Ctrl+C(SIGINT) within 5 Seconds..."<<std::endl;
    sleep(5); //simulate 5 seconds long task, so we can send SIGINT within that frame. 
    std::cout<<"Finished handling SIGUSR1!"<<std::endl;
}

int main(){
    struct sigaction sa;
    sa.sa_handler = handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);

    signal(SIGINT, [](int){std::cout<<"SIGINT receved!"<<std::endl;});

    std::cout<<"PID: "<<getpid()<<"- Send SIGUSR1 signal and press Ctrl+C to send SIGINT signal."<<std::endl;
    pause(); //for SIGUSR1
    pause(); //for SIGINT(if sent)

    return 0;
}