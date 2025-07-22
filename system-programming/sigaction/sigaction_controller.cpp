#include<csignal>
#include<unistd.h>
#include<cstring>
#include<sys/wait.h>
#include<iostream>


int command_pipe[2];
bool running = true;


void signal_handler(int sig, siginfo_t* info, void* context){
    std::cout<<"signal_handler::received signal - "<<sig<<std::endl;
    char buffer[128];
    int n = read(command_pipe[0], buffer, sizeof(buffer));
    buffer[n] = '\0';

    std::cout<<"signal_handler::child::command received - "<<buffer<<std::endl;
    std::cout<<"signal_handler::signal information - PID:"<<info->si_pid<<", UID"<<info->si_uid<<std::endl;

    if(strcmp(buffer, "shutdown")==0){
        std::cout<<"signal_handler::child::shutting down gracefully..."<<std::endl;
        running = false;
    }
}


int main(){
    pipe(command_pipe);
    pid_t pid = fork();

    if(pid==0){
        close(command_pipe[1]);

        struct sigaction sa;
        sa.sa_sigaction = signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &sa, NULL);


        std::cout<<"child::pid - "<<getpid()<<std::endl;
        std::cout<<"child::waiting for commends..."<<std::endl;

        while(running) pause();

        std::cout<<"child::exiting...."<<std::endl;
    }else{
        close(command_pipe[0]);
        sleep(1);

        std::string cmds[] = {"start", "collect", "shutdown"};

        for(const auto& cmd:cmds){
            std::cout<<"parent::sending command - "<<cmd<<std::endl;
            write(command_pipe[1], cmd.c_str(), cmd.length());
            kill(pid, SIGUSR1);
            sleep(1);
        }

        wait(NULL);

        std::cout<<"parent::childe existed!"<<std::endl;
    }

    std::cout<<"parent::exiting..."<<std::endl;

    return 0;
}