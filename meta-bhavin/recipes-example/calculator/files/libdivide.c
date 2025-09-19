#include "calc.h"

float divide(int a, int b){
    if(b==0){
        return 0.0; ///basic error handling
    }
    return (float)a / (float)b;
}