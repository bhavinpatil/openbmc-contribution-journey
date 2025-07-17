#include<csignal> //signal and it's type
#include<unistd.h> //fork(), pause(), sleep, getpid
#include<iostream> // cout, endl
#include<sys/wait.h> //wait
int count = 0;


void handle_signal(int sig){
    std::cout<<"Child Received signal: "<<sig<<std::endl;
    if(sig==SIGUSR1){
        count++;
        std::cout<<"This statment is printig after getting SIGUSR1 from parent!: counting : "<<count<<std::endl;    
    }
    if(sig==SIGTERM){
        std::cout<<"Child: received terminating signal from parent! Bye Bye!."<<std::endl;
        exit(0);
    }
}


int main(){
    pid_t pid = fork();

    if(pid<0){
        perror("fork");
        return 1;
    }

    if(pid == 0){
        // ---------------CHILD Process-----------------
        signal(SIGUSR1, handle_signal);
        signal(SIGTERM, handle_signal);

        std::cout<<"Child PID : "<<getpid()<<std::endl;

        std::cout<<"Waiting for getting signal from Parent!..."<<std::endl;

        while(true){pause();}; //wait for signal to arrived.

        std::cout<<"Child existing gracefully!"<<std::endl;
    }else{
        //----------------PARENT Process-----------------

        std::cout<<"Parent process ID: "<<getpid()<<std::endl;
        std::cout<<"Sending signal to Child PID: "<<pid<<std::endl;

        sleep(1);
        for(int i=0; i<5; i++){
            kill(pid, SIGUSR1);//sending signal to child by calling the kill command using child pid;
            sleep(1); //give time to child to setup
        }


        kill(pid, SIGTERM);

        wait(NULL); //wait for child to finish.

        std::cout<<"Parent: child has existed!"<<std::endl;
    }

    return 0;

}