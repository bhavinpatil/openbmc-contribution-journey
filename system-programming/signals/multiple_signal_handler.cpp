#include<unistd.h>
#include<csignal>
#include<iostream>
#include<sys/wait.h>

void handle_signal(int sig){
    std::cout<<"Child : received signal - "<<sig<<std::endl;
    if(sig==SIGUSR1){
        std::cout<<"Child recieved signal SIGUSR1 : start matrics collectio!"<<std::endl;
    }
    else if(sig==SIGUSR2){
        std::cout<<"Child received signal SIGUSR2 : stop collecting and flush logs"<<std::endl;
    }
}

int main(){
    pid_t pid = fork();

    if(pid==0){
        //child process section
        signal(SIGUSR1, handle_signal);
        signal(SIGUSR2, handle_signal);

        std::cout<<"Child waiting for signals with Child PID: "<<getpid()<<std::endl;

        //wait for two signals
        pause(); //wait for SIGUSR1
        pause(); //wait for SIGUSR2

        std::cout<<"Child done handling both signals! "<<std::endl;
    }
    else{
        // PARENT process section

        sleep(2);
        kill(pid, SIGUSR1);
        sleep(2);
        kill(pid, SIGUSR2);
        wait(NULL);
    }
    return 0;
}