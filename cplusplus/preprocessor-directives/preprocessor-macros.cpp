#include<bits/stdc++.h>
using namespace std;


#define DEBUG 0
#define RELEASE

int main(){
    cout<<"Program started!"<<endl;

    if(DEBUG)
    {
        cout<<"DEBUG mode is ON!"<<endl;
    }else{
        cout<<"DEBUG mode is OFF!"<<endl;
    }
    
    #ifndef RELEASE
    cout<<"This line is printed only if RELEASE is not defined!"<<endl;
    #endif

    #ifdef RELEASE
    cout<<"This line is printed when RELEASE is defined!"<<endl;
    #endif

    cout<<"Program Exited!"<<endl;
    int* n = new int(20);
    
    int* loc = n;
    
    cout<<"N: n -- "<<n<<endl;
    cout<<"N: *n -- "<<*n<<endl;
    cout<<"N: &n -- "<<&n<<endl;
    cout<<"=========================="<<endl;
    cout<<"Loc: Loc -- "<<loc<<endl;
    cout<<"Loc: &Loc -- "<<&loc<<endl;
    cout<<"Loc: *Loc -- "<<*loc<<endl;
    
    return 0;
}