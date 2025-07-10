#include<bits/stdc++.h>
using namespace std;

int main(){
    
    vector<string>logBuffer;

    string input;

    cout<<"Enter log message (type 'exit' to quit):"<<endl;

    while(true){
        getline(cin, input);

        if(input=="exit") break;

        logBuffer.push_back(input);

        if(logBuffer.size()==10){
            ofstream outfile("log.txt", ios::app);

            for(const auto& msg:logBuffer){
                outfile << msg << endl;
            }
            logBuffer.clear();
        } 
    }
    return 0;

}