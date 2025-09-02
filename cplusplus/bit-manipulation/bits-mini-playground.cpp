#include<bits/stdc++.h>
using namespace std;


int main(){
    cout<<"=========================="<<endl;
    cout<<"=========================="<<endl;
    cout<<"Checking the 3rd bit from right most side is Set or Not Set!\n n=10, k=3"<<endl;
    int k = 3;
    
    int n = 10;
    
    cout<<bitset<6>(n)<<":("<<n<<")"<<endl;
    
    if(n&(1<<k)){
        cout<<"Bit is set"<<endl;
    }else{
        cout<<"Bit is not set"<<endl;
    }
    cout<<"=========================="<<endl;
    cout<<"=========================="<<endl;
    
    cout<<"Toggling the 0th bit i.e. right most bit for n=10\n"<<endl;
    int s = n^(1<<0);
    
    cout<<bitset<8>(s)<<":("<<s<<")"<<endl;
    cout<<"=========================="<<endl;
    cout<<"=========================="<<endl;
    
    cout<<"Left Shirt & Right Shirt operation on num=11\n"<<endl;
    int num = 11;
    cout<<"Num: "<<bitset<8>(num)<<":"<<num<<endl;
    cout<<"------------------"<<endl;
    num = num<<1;
    cout<<"This is left shirt by 1(multiplies by 2^k i.e. k=1): "<<bitset<8>(num)<<":"<<num<<endl; 
    cout<<"------------------"<<endl;
    num = num>>2;
    cout<<"This is right shirt by 2(divides by 2^k i.e. k=2): : "<<bitset<8>(num)<<":"<<num<<endl;
    cout<<"------------------"<<endl;
    cout<<"=========================="<<endl;
    
    cout<<"Counting set bits i.e. 1s in num\n"<<endl;
    int count = 0;
    int x=num;
    while(x){
        cout<<bitset<8>(x)<<":"<<x<<endl;
        cout<<bitset<8>(x-1)<<":"<<x-1<<endl;
        cout<<"------------------"<<endl;
        x = x&x-1;
        cout<<bitset<8>(x)<<":"<<x<<endl;
        cout<<"=========================="<<endl;
        count++;
    }
    cout<<"Bit Counts in "<<num<<":"<<count<<endl;
    cout<<"=========================="<<endl;
    cout<<"=========================="<<endl;

    cout<<"Toggle all bits using ~ for num2=10\n"<<endl;
    int num2 = 10;
    cout<<bitset<8>(num2)<<":"<<num2<<endl;
    cout<<bitset<8>(~num2)<<":"<<~num2<<endl;
    cout<<"We can use ~ to toggle all the bits as it will turn all 0s in 1s which is wrong so \nwe use masking XOR with mask, toggling kth bit\n"<<endl;
    int nu = num2^(1<<4);
    cout<<"Toggled 4th bit in num2=10\nBefore toggling: "<<bitset<8>(num2)<<":"<<num2<<endl;
    cout<<"After toggling: "<<bitset<8>(nu)<<":"<<nu<<endl;
    cout<<"=========================="<<endl;
    cout<<"=========================="<<endl;
    
    
    return 0;

}