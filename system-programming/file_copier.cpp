#include<fcntl.h>
#include<unistd.h>
#include<iostream>

int main(){
    int sourceFD = open("source.txt", O_RDONLY); //opens source file for reading

    if(sourceFD==-1){
        perror("open source");
        
        return 1;
    }

    int destFD = open("dest.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); //open file writing | creates file if doesnot exist
    //| truncate file to 0 bytes if exists | 0644 - file permission → rw-r--r-- (user can read/write; group/others can read)
    
    if(destFD == -1){
        perror("open dest");
        close(destFD);
        return 1;
    }

    char buffer[128];
    int byteRead;

    std::cout<<"Source File Descriptor: "<<sourceFD<<std::endl;
    std::cout<<"Destination File Descriptor: "<<destFD<<std::endl;
    
    while((byteRead = read(sourceFD, buffer, sizeof(buffer)))>0){
        write(destFD, buffer, byteRead);
    }

    close(destFD);
    close(sourceFD);
    return 0;
}