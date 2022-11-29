// #include<bits/stdc++.h>
// using namespace std;
// #include"PCB.hpp"
// PCB pcb;
// vector<char> IR;
// int IC=0;    
// string appendErrorMess(int errorNum,string line){
//     cout<<"Inside appendErrorMess()"<<endl;
//     switch (errorNum)
//     {
//     case 0:
//         line += "No Error";
//         break;
//     case 1:
//         line += "Out of Data";
//         break;
//     case 2:
//         line += "Line Limit Exceeded";
//         break;
//     case 3:
//         line += "Time Limit Exceeded";
//         break;
//     case 4:
//         line += "Operation Code Error";
//         break;
//     case 5:
//         line += "Operand Error";
//         break;
//     case 6:
//         line += "Invalid Page Fault";
//         break;
//     default:
//         break;
//     }
//     return line;
// }
// /// @brief terminate with error message
// void Terminate(int error1,int error2)
// {
//     cout << "Inside Terminate() ....\n";
//     string line = "";
//     line += appendErrorMess(error1,"");
//     if(error2!=-1){
//         line += " and ";
//         line += appendErrorMess(error2,"");
//     }
//     ofstream file2("linePrinter.txt", ios::app);
//     if (file2.is_open())
//     {
//         file2 << "JOB ID : " << pcb.getJID() << endl;
//         file2 << line << endl;
//         file2 << "IC: " << IC << endl;
//         file2 << "IR: " << IR[0] << IR[1] << IR[2] << IR[3] << endl;
//         file2 << "TTC: " << pcb.getTTC() << endl;
//         file2 << "LLC: " << pcb.getLLC() << endl;
//         file2.close();
//     }
// }
// int main(){
//     pcb.setJID(10);
//     pcb.setTLL(11);
//     pcb.setTTL(12);
//    // cout<<a.getJID()<<endl;
//    // cout<<a.getTLL()<<endl;
//    // cout<<a.getTTL()<<endl;
//     IR.push_back('1');
//     IR.push_back('1');
//     IR.push_back('2');
//     IR.push_back('2');  


//     Terminate(2,-1);
//     // vector<vector<char>> M(30,vector<char>(4,'-'));
//     // string inst;
//     // cout<<"Input instruction: ";
//     // getline(cin,inst);
//     // int add=0,inn=0,n=inst.size();
//     // for(int i=0;i<n;i++){
//     //     if(inst[i]=='H'){
//     //         M[add][inn]=inst[i];
//     //         inn=0;
//     //         add++;
//     //         continue;
//     //     }
//     //     M[add][inn]=inst[i];
//     //     inn++;
//     //     if(inn==4){
//     //         add++;
//     //         inn=0;
//     //     }
//     // }
//     // add=0,inn=0;
//     // for(int i=0;i<n;i++){
//     //     if(inst[i]=='H'){
//     //        cout<<M[add][inn]<<endl;
//     //         inn=0;
//     //         add++;
//     //         continue;
//     //     }
//     //     for(int j=0;j<=3;j++){
//     //         cout<<M[add][j]<<" ";
//     //         i++;
//     //     }cout<<endl;
//     //     add++;
//     //     i--;
//     // }
//     return 0;
    
// }