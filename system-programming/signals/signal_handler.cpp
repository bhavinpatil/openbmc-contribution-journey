#include<csignal> // for signal and types i.e. SIGUSR1
#include<unistd.h> // for pause() and geting pid
#include<iostream> // printing statements;


void handle_signal(int sig){
    std::cout<<"Received Signal: "<<sig<<std::endl;
    if(sig==SIGUSR1){
        std::cout<<"Custom action using User Signal : logging metrics...  "<<std::endl;
    }
    if(sig==SIGTERM){
        std::cout<<"Terminating the process using signal received from other termianl! Bye bye."<<std::endl;
        exit(0);
    }
}


int main(){
    signal(SIGUSR1, handle_signal);
    signal(SIGTERM, handle_signal);


    std::cout<<"Process ID: "<<getpid()<<std::endl;
    std::cout<<"Waiting for SIGUSR1... (press Ctrl to quit.)"<<std::endl;

    while(true){
        pause(); //keeps running the process and waits for signal so when receivde it will call the hander and prints the statement.
    }
    return 0;
}