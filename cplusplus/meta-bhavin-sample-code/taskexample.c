#include<stdio.h>
#include "taskexample.h"  

int main(){
    #ifdef WELCOME
        #if WELCOME == y
            printf("Welcome to OpenBMC Contrinution Learning journey!\n");
        #endif
    #endif

    #ifdef HELLO
        #if HELLO == y
            printf("Hello World! from Original Source code for HELLO!\n");
        #endif
    #endif

    #ifdef BYE
        #if BYE == y
            printf("Bye, keep learning and practicing!\n");
        #endif
    #endif

    #ifdef THANKYOU
        #if THANKYOU == y
            printf("Thank you for your time!\n");
        #endif
}