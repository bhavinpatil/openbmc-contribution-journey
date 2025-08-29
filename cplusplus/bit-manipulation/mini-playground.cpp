#include<bits/stdc++.h>

int main(){

    int num = 10;
    std::cout<<"Number: "<<num<<std::endl;
    std::cout<<"Binary Number: "<<std::bitset<8>(num)<<std::endl;
    num = num>>1;
    std::cout<<"Number: "<<num<<std::endl;
    std::cout<<"Binary Number: "<<std::bitset<8>(num)<<std::endl;
    
    std::cout<<"======================================="<<std::endl;

    num = 10;
    std::cout<<"Number: "<<num<<std::endl;
    std::cout<<"Binary Number: "<<std::bitset<8>(num)<<std::endl;
    num = num>>10;
    std::cout<<"Number: "<<num<<std::endl;
    std::cout<<"Binary Number: "<<std::bitset<8>(num)<<std::endl;
    
    num = 13;
    
    int count = 0;    
    while(num){
        num = num&(num-1);
        count++;
    }

    std::cout<<count<<std::endl;

    int num1 = 64;
    std::cout<<std::bitset<8>(num1)<<std::endl;
    std::cout<<std::bitset<8>(num1&num1-1)<<std::endl;
    if(num1>0 && (num1&(num1-1))==0)
    std::cout<<"Power of two! "<<num1<<std::endl;
    else
    std::cout<<"No Power of two!"<<num1<<std::endl;
    
    int a = 12;
    int b = 13;
    std::cout<<"A: "<<a<<" B: "<<b<<std::endl;
    a = a^b;
    b = b^a;
    a = a^b;
    std::cout<<"A: "<<a<<" B: "<<b<<std::endl;
    
    std::cout<<std::bitset<8>(a)<<"("<<a<<")"<<std::endl;
    int k = 0;
    a = a^(1<<k);
    std::cout<<"A: "<<std::bitset<8>(a)<<"("<<a<<")"<<std::endl;
    std::cout<<"B: "<<std::bitset<8>(b)<<"("<<b<<")"<<std::endl;
    
    return 0;
}