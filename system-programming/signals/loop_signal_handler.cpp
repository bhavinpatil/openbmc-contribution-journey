#include<unistd.h>
#include<csignal>
#include<iostream>
#include<sys/wait.h>

int signal_count = 0;

void handle_signal(int sig){
    std::cout<<"Signal received: "<<sig<<std::endl;
    signal_count++;
}

int main(){
    pid_t pid = fork();

    if(pid==0){
        signal(SIGUSR1, handle_signal);

        std::cout<<"Child PID: "<<getpid()<<" - Waiting for signals..."<<std::endl;

        while(signal_count<3){
            pause();
        }

        std::cout<<"Child: Handled 3 signals. Exitig..."<<std::endl;
    }else{
        for(int i=0; i<5; i++){
            sleep(1);
            kill(pid, SIGUSR1);
        }
        wait(NULL);
    }
    return 0;
}