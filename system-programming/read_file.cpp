#include<fcntl.h>
#include<unistd.h>
#include<iostream>

int main(){
    int fd = open("file1.txt", O_RDONLY);
    if(fd==-1){
        perror("open");
        return 1;
    }

    std::cout<<"File Descriptor: "<<fd<<std::endl;

    char buffer[8];
    int byteRead;

    while((byteRead = read(fd, buffer, sizeof(buffer)))>0){
        write(STDOUT_FILENO, buffer, byteRead);
    }

    close(fd);
    return 0;
}