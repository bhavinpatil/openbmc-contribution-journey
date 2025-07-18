#include<csignal>
#include<iostream>
#include<unistd.h>

int count = 0;


void handle_signal(int sig){
    count++;
    std::cout<<"Signal recieved: "<<sig<<"\nCount: "<<count<<std::endl;
}

int main(){

    // signal(SIGUSR1, handle_signal);


    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);

    std::cout<<"PID: "<<getpid()<<std::endl;

    while(count<3) pause();

    std::cout<<"Exiting after count == 3"<<std::endl;
    return 0;
}