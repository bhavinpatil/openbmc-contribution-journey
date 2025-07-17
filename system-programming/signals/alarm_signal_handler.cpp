#include<csignal>
#include<unistd.h>
#include<iostream>
#include<sys/wait.h>

void alarm_handler(int sig){
    std::cout<<"Alarmed triggered after delay (SIGALRM received!)"<<std::endl;
}

int main(){
    signal(SIGALRM, alarm_handler);

    std::cout<<"Setting alarm for 5 seconds..."<<std::endl;
    alarm(5);

    while(true){
        pause();
    }

    return 0;
}