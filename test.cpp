#include<bits/stdc++.h>
using namespace std;
int main(){
    vector<vector<char>> M(30,vector<char>(4,'-'));
    string inst;
    cout<<"Input instruction: ";
    getline(cin,inst);
    int add=0,inn=0,n=inst.size();
    for(int i=0;i<n;i++){
        if(inst[i]=='H'){
            M[add][inn]=inst[i];
            inn=0;
            add++;
            continue;
        }
        M[add][inn]=inst[i];
        inn++;
        if(inn==4){
            add++;
            inn=0;
        }
    }
    add=0,inn=0;
    for(int i=0;i<n;i++){
        if(inst[i]=='H'){
           cout<<M[add][inn]<<endl;
            inn=0;
            add++;
            continue;
        }
        for(int j=0;j<=3;j++){
            cout<<M[add][j]<<" ";
            i++;
        }cout<<endl;
        add++;
        i--;
    }
    return 0;
    
}