/*
Phase-1 && Phase-2 combining notations
M: memory- *
IR: Instruction Register -*
C: Instruction Counter Register -*
R:General Purpose Register -*
C:Toggle-*
Buffer-*
*/
/*
Phase-2 extra stuff
INTERRUPT VALUES
    SI = 1 on GD
    = 2 on PD
    = 3 on H
    TI = 2 on Time Limit Exceeded
    PI = 1 Operation Error
    = 2 Operand Error
    = 3 Page Fault
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "PCB.hpp"
#include <cstdlib> // for Random Number generation
#include<set>
#include<time.h>
using namespace std;

//------------------------------
vector<vector<char>> M;
vector<char> IR;
vector<char> R;
int IC;
bool C;
vector<vector<char>> buffer;
int buffSize;
// -----------------------------
// -------------PHASE-2- STUFF---------------------------------
int PTR; //(Page Table pointer);
// process control block
PCB pcb;
// set for allocated frames
vector<int> allocatedFrames;
// Setting interrupt
int SI = 3, TI = 0, PI=0;
int frame; // frame number;
int numGD=0;
int numBuff=0;
// ------------------------Declaration of functions ...------------------------------------
//21 functions are there
// Append errors
string appendErrorMess(int errorNum,string line);
/// @brief WHEN ERROR Occured then execute
/// @param error1 
/// @param error2 
void Terminate(int error1,int error2);
int addressMap(int VA);
void getFrameForProgram();
void evalGD(int add);    
void evalPD(int add);    
void evalH();
bool MOS1(int add);
bool isValidPF();
void RectifyPF(int va);
bool MOS2();
void loadRegister(int address);
void storeRegister(int address);
void compareRegister(int address);
void branchOnto(int instNum);
bool userProgram(int totalInst);
int storeInstruction(string inst);
bool startWith(string s, string prefix);
int ALLOCATE();
void createPageTable();
void INIT(int id,int tl,int ll);
int excuteAMJ(string line);
void createMemoryFile();
// -----------------------------------------------------------------------------------------
// Creating Memory file
void createMemoryFile()
{
    ofstream myfile;
    myfile.open("MemoryFile.txt", ios::app);
    for (int i = 0; i < 300; i++)
    {
        string line;
        for (int j = 0; j <= 3; j++)
        {
            line += M[i][j];
        }
        myfile << line;
        myfile << "\n";
    }
    myfile << "\n";
    myfile << "\n";
    myfile.close();
}
/// @brief GD
/// @param address
void evalGD(int add)
{
    cout << "Evaluating GD....\n";
    int n = buffSize;
    cout << "Buffer size: " << n << endl;
    for (int k = 0; k < n; k++)
    {
        cout<<"Storing buffer in mainMem.."<<endl;
        for (int i = 0; i <= 3; i++)
        {
            M[add + k][i] = buffer[k][i];
            // cout << "Buffer[" << k << "][" << i << "]"
            //      << ": " << buffer[k][i] << endl;
        }
    }
    cout << "Whole memory after GD: " << endl;
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}
void evalPD(int add)
{
    cout << "Evaluating PD....\n";
    ofstream file2("linePrinter.txt", ios::app);
    // int n = buffSize;
    if (file2.is_open())
    {

        string l = "";
        for (int i = add; i < add + 10; i++)
        {
            int count = 0;
            for (int k = 0; k <= 3; k++)
            {
                if (M[i][k] == '-')
                {
                    l += ' ';
                    // count++;
                    // if (count == 4)
                    // {
                    //     break;
                    // }
                    continue;
                }
                l += M[i][k];
            }
            // if (count == 4)
            // {
            //     break;
            // }
        }
        file2 << l;
        file2 << "\n";
        file2.close();
    }
    cout << "Successfully print the content on Reader\n";
}
/// @brief Terminate
void evalH()
{
    cout << "Evaluating H....\n";
    ofstream file2("linePrinter.txt", ios::app);
    file2 << "\n";
    file2 << "\n";
    file2.close();
}
/// @brief TI AND SI
bool MOS1(int add)
{
    cout << "In the MOS1...\n";
    //Return : True - when have some error except H
    // Return: False- when have no error
    if(TI==0 && SI==1){   //
        evalGD(add);
        return false;
    }
    else if(TI==0 && SI==2){
        evalPD(add);
        return false;
    }
    else if(TI==0 && SI==3){
        Terminate(0,-1);
        // evalH();
        return true;
    }
    else if(TI==2 && SI==1){
        Terminate(3,-1);
        return true;
    }
    else if(TI==2 && SI==2){
        // After writing terminate 
        evalPD(add);
        Terminate(3,-1);
        return true;
    }
    else if(TI==2 && SI==3){
        Terminate(0,-1);
        return true;
    }
    // have some error 
    return true;
}
bool isValidPF(){
  cout<<"Inside isValidPF()"<<endl;
  // Page fault for GD and SR 
  if(IR[0]=='G' && IR[1]=='D'){
    return true;
  }
  if(IR[0]=='S' && IR[1]=='R'){
    return true;
  }
  // For rest of the 5 operation their is no page fault 
  return false;
}
void RectifyPF(int va){
    cout<<"Inside  RectifyPF()"<<endl;
    int PTE = PTR*10 + va/10;
    int val=ALLOCATE();
    while(allocatedFrames[val]){  // checking whether the frame is allocated or not 
        val=ALLOCATE();
    }
    // keeping track of allocated frames... 
    allocatedFrames[val]++;
    cout<<"New Page is :" << val<<endl;
    cout<<"Page is store AT: "<<PTE<<endl;
    cout<<val/10 <<val%10<<endl;
    M[PTE][2] =  ((val/10)+'0');
    M[PTE][3] = ((val%10)+'0');
    cout<<"M[PTE][2]: "<<M[PTE][2]<<endl;
    cout<<"M[PTE][3]: "<<M[PTE][3]<<endl;
}
/// @brief MOS 2 IS FOR TI AND PI
/// @return 
bool MOS2()
{
    cout << "In the MOS2...\n";
    // Interrupts...
    if(TI==0 && PI==1){
       Terminate(4,-1);
       return true;
    }
    else if(TI==0 && PI==2){
       Terminate(5,-1);
       return true;
    }
    else if(TI==0 && PI==3){
        if(isValidPF()){
            return false;
        }
        Terminate(6,-1);
        return true;
    }
    else if(TI==2 && PI==1){
      Terminate(3,4);
      return true;
    }
    else if(TI==2 && PI==2){
       Terminate(3,4);
       return true;
    }
    else if(TI==2 && PI==3){
       if(isValidPF()){
        Terminate(3,-1);
        return true;
       }
       Terminate(3,6);
       return true;
    }
    // TI==0 && PI==0
    return false;
    
}

// --------------------------
/// @brief userMode/kernel mode
void loadRegister(int address)
{
    for (int i = 0; i < 4; i++)
    {
        R[i] = M[address][i];
    }
    cout << "Register is : ";
    for (int i = 0; i < 4; i++)
    {
        cout << R[i] << " ";
    }
    cout << endl;
}
void storeRegister(int address)
{
    for (int i = 0; i < 4; i++)
    {
        M[address][i] = R[i];
    }
    cout << "store in mem: ";
    for (int i = 0; i < 4; i++)
    {
        cout << M[address][i] << " ";
    }
    cout << endl;
}
void compareRegister(int address)
{
    for (int i = 0; i < 4; i++)
    {
        if (M[address][i] != R[i])
        {
            return;
        }
    }
    C = true; // setting toggle true
}
void branchOnto(int instNum)
{
    if (C)
    {
        IC = instNum - 1;
    }
}
// --------------------------
/// @brief Control card-- $DTA and $End-Stop
bool userProgram(int totalInst)
{
    cout << "Inside userProgram" << endl;
    
    bool nextBuffer = false;
    int add =  addressMap(IC);
    // 
    // Non contiguous memory allocation so after 10 instruction need to check for further page.
    while (IC < totalInst)
    {
        if(add==-1){
            cout<<"Page is not allocated..."<<endl;
            return false;
        } 
        // Loading instruction into IR
        IR = M[add];
        if (IR[0] == 'H')
        {
            cout<<"In the H..."<<endl;
            pcb.incrementTTC();
            SI = 3;
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            // to check TTC
            if(MOS2()){
                cout<<"Error occured while executing H..."<<endl;
               return false; // error occured in User Program
            } 
            if(MOS1(-1)){
                return false;
            }    
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            cout<<"In the GD..."<<endl;
            if (nextBuffer)
            {
                   // need data.....
                return true; // no error demand for the next buffer
            }
            pcb.incrementTTC();   
            pcb.incrementTTC();
          // Check oprand Error   GDPH  --- P - 0 >9
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            if(MOS2()){ // if mos2 is giving any error then program terminate abnormally
                cout<<"Error occured while executing GD..."<<endl;
                return false; 
            }
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);
            cout<<"ra: "<<ra<<endl;
            if(ra==-1){
               cout<<"Valid Page fault in GD"<<endl;
                PI=3;
            }
      
            if(MOS2()){ // if mos2 is giving any error then program terminate abnormally
                return false; 
            }
            // Handle page fault error
            if(PI==3 && isValidPF()){
                cout<<"Assiging new Page in GD..."<<endl;
                RectifyPF(va);
                ra = addressMap(va); // mapping address again..
            }
            cout<<"Real address after rectifying page Fault: "<<ra<<endl;
            // MOS1(TI,SI)
            SI = 1; // setting interrupt
            if(MOS1(ra)){ // with real address
                return false; // error .....
            }
            PI=0; // because page fault is resolved......GD
            nextBuffer = true;
            numGD++;
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            cout<<"In the PD..."<<endl;
            pcb.incrementLLC();
            pcb.incrementTTC();
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            // Handling error 
            if(MOS2()){
                return false;
            }
            // opcode error
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);
            if(ra==-1){
                //
                cout<<"Occuring Page fault in PD"<<endl;
                PI=3;
            }
            //Handling and error 
            if(MOS2()){
                return false; // error user program have 
            }
            
            // LLC
            if(pcb.isLLCExceed()){
                Terminate(2,-1);
                return false;
            }
            // LLC ERROR HANDLING 
            SI = 2; // setting interrupt
            if(MOS1(ra)){ 
                return false; // erro user program have
            } 
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            cout<<"In the LR..."<<endl;
            pcb.incrementTTC();
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            if(pcb.isTTCExceed()){
                TI=2;
            }
            if(MOS2()){ // Handling error 
                return false;
            }
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);
            // TTC
            if(ra==-1){
                PI=3;
            }
            if(MOS2()){ // Handling error 
                return false;
            }
            loadRegister(ra);
            // TODO-8: Handle error of if frame is already allocated to some other page
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            cout<<"In the SR..."<<endl;
            pcb.incrementTTC();
            pcb.incrementTTC(); // sr need 2 unit time 
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            if(MOS2()){
                return false;
            }
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);  // change to real address
            if(ra==-1){
                PI=3;
            }
            if(MOS2()){ // checking for page fault error
                return false;
            }
            // Handle page fault error
            if(PI==3 && isValidPF()){
                cout<<"Rectifying Page Fault.."<<endl;
                RectifyPF(va);
                ra = addressMap(va); // mapping address again..
            }
            storeRegister(ra); 
            PI=0; // because page fault is resolved......SR
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            cout<<"In the CR..."<<endl;
            pcb.incrementTTC();
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            if(MOS2()){
                return false; // there is some error
            }
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);
            if(ra==-1){
                PI=3;
            }
            if(MOS2()){ // PI ==3 // error is there .... 
                return false; // there is some error
            }
            compareRegister(ra); //  TODO :  to change 
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            cout<<"In the BT..."<<endl;
            pcb.incrementTTC();
            if((IR[2] - '0')<0 || (IR[2] - '0')>9){
                  PI =2;
            }
            if((IR[3] - '0')<0 || (IR[3] - '0')>9){
                 PI=2; 
            }
            // TTC
            if(pcb.isTTCExceed()){
                TI=2;
            }
            if(MOS2()){
                return false; // there is some error
            }
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            int ra = addressMap(va);
           
            if(ra==-1){
                PI=3;
            }
            if(MOS2()){
                return false; // there is some error
            }
            branchOnto(ra); //  TODO :  to change 
        }
        add++;  // moving on further instruction ....
        IC++; // keeping track of number of instruction
        if(IC%10==0){
            // check for other pages
            add = addressMap(IC);
        }
    }
    return true;
}
/// @brief Program card
// works fine...
int storeInstruction(string inst)
{
    cout << "Inside storeInstruction() " << endl;
    int i = 0, n = inst.size();
    cout<<"Size of all instructions "<<n<<endl;
    while(i<n){
        int add = addressMap(IC);// initially IC is 0 
        if(inst[i]=='H'){
            cout<<"H-Storing"<<endl;
            pcb.incrementTTC();
            M[add][0] = inst[i];
            IR = M[add];
            // then store in memory
            // TTC is exceeded its just loading
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            i++;
        }
        else if((i!=n-1)&&(inst[i]=='G' && inst[i+1]=='D')){
           // increment TTL
            cout<<"GD-Storing"<<endl;
            pcb.incrementTTC();
            pcb.incrementTTC();
            if(i+3>n){
                cout<<"There is some Unknown ERROR while running GD..."<<endl;
                return -1;
            }
            //Need to store data in instruction register to check for valid or invalid page fault;
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];// just for the output
            // TTC is exceeded its just loading so no need to check for Any error except the operation one ...
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            
            i += 4;
        }
        else if((i!=n-1)&&(inst[i]=='P' && inst[i+1]=='D')){
            cout<<"PD-Storing"<<endl;
            pcb.incrementLLC();
            pcb.incrementTTC();
            //Need to store data in instruction register:
            if(i+3>n){
                cout<<"There is some Unknown ERROR while running PD..."<<endl;
                return -1;
            }
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];
            // TTC is exceeded
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            // if(pcb.isLLCExceed()){
            //     Terminate(2,-1);
            //     return -1;
            // }
            i += 4;
        }
        else if((i!=n-1)&&(inst[i]=='L' && inst[i+1]=='R')){
            cout<<"LR-Storing"<<endl;
            pcb.incrementTTC();
           //Need to store data in instruction register:
            if(i+3>n){
                cout<<"There is some Unknown ERROR while running LR..."<<endl;
                return -1;
            }
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];
           // TTC is exceeded its just loading
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            i += 4;
        }
        else if((i!=n-1)&&(inst[i]=='S' && inst[i+1]=='R')){
            cout<<"SR-Storing"<<endl;
            pcb.incrementTTC();
             if(i+3>n){
                cout<<"There is some Unknown ERROR while running SR..."<<endl;
                return -1;
            }
            //Need to store data in instruction register:
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];
            // TTC is exceeded its just loading
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            i += 4;
        }
        else if((i!=n-1)&&(inst[i]=='C' && inst[i+1]=='R')){
            cout<<"CR-Storing"<<endl;
           pcb.incrementTTC();
            if(i+3>n){
                cout<<"There is some Unknown ERROR while running CR..."<<endl;
                return -1;
            }
           //Need to store data in instruction register:
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];
            // TTC is exceeded its just loading
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            i += 4;
        }
        else if((i!=n-1)&&(inst[i]=='B' && inst[i+1]=='T')){
            cout<<"BT-Storing"<<endl;
            pcb.incrementTTC();
             if(i+3>n){
                cout<<"There is some Unknown ERROR while running BT..."<<endl;
                return -1;
            }
            //Need to store data in instruction register:
            M[add][0] = inst[i];
            M[add][1] = inst[i+1];
            M[add][2] = inst[i+2];
            M[add][3] = inst[i+3];
            IR = M[add];
            // TTC is exceeded its just loading
            // if(pcb.isTTCExceed()){
            //     TI=2;
            //     Terminate(3,-1);
            //     return -1;
            // }
            i += 4;
        }
        else{
            int  till = i+4,k=0;
            while(i<till && i<n){
                IR[k]=inst[i];
                i++;
                k++;
            }
            cout<<"Making PI==1"<<endl; 
            PI=1;
            pcb.incrementTTC();
            if(pcb.isTTCExceed()){
            TI = 2;      
            }
            break;
        }
        IC++;
        if(IC%10==0){
            getFrameForProgram();
            frame++; //incrementing frame size 
        }
    }
    int totalInst = IC;
    cout<<"Total number of instruction: "<<IC<<endl;
    //Handle error PI=1 and TI=2
    if(MOS2()){
        return -1;
    }
    cout << "After store instruction() memory looks like : " << endl;
    for(int i=0;i<300;i++){
        for(int j=0;j<4;j++){
            cout<<M[i][j]<<" ";
        }cout<<endl;
    }
    
    IC=0; //setting IC
    // because i increment it at time of storing instruction
    pcb.setLLC(0);
    pcb.setTTC(0);
    IR = vector<char>(4,'-');
    return totalInst; 
}

/// @brief To check prefix
bool startWith(string s, string prefix)
{
    int n = prefix.length();
    for (int i = 0; i < n; i++)
    {
        if (s[i] != prefix[i])
        {
            return false;
        }
    }
    return true;
}
// -------------PHASE-2- STUFF---------------------------------
int ALLOCATE()
{
    // srand(time(0));
    // rand() generate pseudo-random number in the range of [0, RAND_MAX)
    int num = rand() % 30;
    return num;
}
/// @brief its mapping the virtual address--> real address
/// @param VA
int addressMap(int VA)
{
    int PTE = (PTR*10) + VA/10;
    cout<<"PTE: "<<PTE<<endl;
    // If there is * present on 2th index of M[PTE] then it means page is not allocated for program or data;
    if(M[PTE][2]=='*'){
        cout<<"No Page is there..."<<endl;
        return -1;
    }
    cout<<"M[PTE][2]-'0': "<<M[PTE][2]-'0'<<endl;
    cout<<"(M[PTE][3]-'0'): "<<(M[PTE][3]-'0')<<endl;
    int add = (M[PTE][2]-'0')*10 + (M[PTE][3]-'0');
    cout<<"add: "<<add<<endl;
    int ra = (add)*10 + VA%10;
    cout<<"The ra is: "<<ra<<endl;
    return ra;
}
/// @brief Append error
/// @param errorNum 
/// @param line 
string appendErrorMess(int errorNum,string line){
    cout<<"Inside appendErrorMess()"<<endl;
    switch (errorNum)
    {
    case 0:
        line += "No Error";
        break;
    case 1:
        line += "Out of Data";
        break;
    case 2:
        line += "Line Limit Exceeded";
        break;
    case 3:
        line += "Time Limit Exceeded";
        break;
    case 4:
        line += "Operation Code Error";
        break;
    case 5:
        line += "Operand Error";
        break;
    case 6:
        line += "Invalid Page Fault";
        break;
    default:
        break;
    }
    return line;
}
/// @brief terminate with error message
void Terminate(int error1,int error2)
{
    cout << "Inside Terminate() ....\n";
    string line = "";
    line += appendErrorMess(error1,"");
    if(error2!=-1){
        line += " and ";
        line += appendErrorMess(error2,"");
    }
    ofstream file2("linePrinter.txt", ios::app);
    if (file2.is_open())
    {
        file2 << "JOB ID : " << pcb.getJID() << endl;
        file2 << line << endl;
        file2 << "IC: " << IC << endl;
        file2 << "IR: " << IR[0] << IR[1] << IR[2] << IR[3] << endl;
        file2 << "TTC: " << pcb.getTTC() << endl;
        file2 << "LLC: " << pcb.getLLC() << endl;
        file2 << "\n\n"<<endl;
        file2.close();
    }
    cout<<"Page Table looks like: "<<endl;
    for(int i=PTR*10;i<PTR*10+10;i++){
        for(int j=0;j<4;j++){
            cout<<M[i][j]<<" ";
        }cout<<endl;
    }
    // creating mainMem file
    createMemoryFile();
}
// ------------------------------------------------------------

/// @brief Page table is creating
void createPageTable(){
    cout<<"Inside createTable() "<<endl;
    int val = PTR*10;
    for(int i=val;i<val+10;i++){
        for(int j=0;j<4;j++){
            M[i][j] = '*';
        }
    }
    for(int i=val;i<val+10;i++){
        for(int j=0;j<4;j++){
            cout<<M[i][j]<<" ";
        }cout<<endl;
    }
}
/// @brief Control card -- $AMJ
void INIT(int id,int tl,int ll)
{
    cout << "Inside INIT()" << endl;
    M = vector<vector<char>>(300, vector<char>(4, '-')); //In phase-2 its 300 size
    IR = vector<char>(4, '-');
    R = vector<char>(4, '-');
    IC = 0;
    C = false;
    buffer = vector<vector<char>>(10, vector<char>(4, '-')); // buffer memory 
    buffSize = 0; // buffer size
    // Phase-2 stuff below...
    // initializing pcb
    //Initaliizing allocated frames to keep track of allocated frames...(random number..)
    allocatedFrames = vector<int>(30,0);
    // to keep track of number of frames...
    frame = 0 ;
    // setting JID ,TTL and TLL... 
    pcb.setJID(id);
    pcb.setTTL(tl);
    pcb.setTLL(ll);
    cout<<"JOBID : "<<pcb.getJID()<<endl;
    cout<<"TTL : "<<pcb.getTTL()<<endl;
    cout<<"TLL : "<<pcb.getTLL()<<endl;
    // initializing ptr and creating page table;
    PTR = ALLOCATE();
    allocatedFrames[PTR]++;
    createPageTable();
    cout<<"PTR : "<<PTR<<endl;
    // initiallizing numGD and numBuffer-- 
    // for out of data error ..... if numGD>numBuff ---> then error
    numGD=0;
    numBuff=0;
    // initiallizing TI,PI AND SI
    TI = 0;
    PI=0;
    SI=3;
}
int excuteAMJ(string line)
{
    cout<<"Inside excuteAMJ"<<endl;
    int n = line.length();
    if (n != 16)
    {
        cout << "Your contol card is invalid..." << endl;
        // execute  with error  so return 0
        return 0;
    }
    //Finding JID ,TTL AND TLL
    int id = 0, tl = 0, ll = 0, i = 4;
    int count = 4; // to keep track of 4 element
    while (count--)
    {
        id = id * 10 + (line[i] - '0');
        i++;    
        // here instead of int any char can be present;
        // TODO-1--- HANDLE THAT ERROR ALSO
    }
    count = 4;
    while (count--)
    {
        tl = tl * 10 + (line[i] - '0');
        i++;
        // here instead of int any char can be present;
        // TODO-2--- HANDLE THAT ERROR ALSO
    }
    count = 4;
    while (count--)
    {
        ll = ll * 10 + (line[i] - '0');
        i++;
        // here instead of int any char can be present;
        // TODO-3--- HANDLE THAT ERROR ALSO
    }
    //calling INIT function which intialize everything.. 
    INIT(id,tl,ll);
    // execute  successfully so return 1
    return 1; 
}
void getFrameForProgram(){
    cout<<"Inside getFrameForProgram()"<<endl;
    int val=ALLOCATE();
    while(allocatedFrames[val]){  // checking whether the frame is allocated or not 
        val=ALLOCATE();
    }
    allocatedFrames[val]++;
    cout<<"Frame No is :"<<val<<endl;
    M[PTR*10+frame][2] = (val/10 + '0');
    M[PTR*10+frame][3] = (val%10 + '0');
    cout<<"Page table look likes :"<<endl;
    for(int i=PTR*10;i<(PTR*10)+10;i++){
        for(int j=0;j<4;j++){
            cout<<M[i][j]<<" ";
        }cout<<endl;
    }
}
int main()
{
    string line;
    ifstream file("Reader.txt");
    if (file.is_open())
    {
        string inst = "";
        while (!file.eof())
        {
            getline(file, line);
            cout<< line << endl;
            if (startWith(line, "$AMJ"))
            {
                int val = excuteAMJ(line);
                // checking successfully executed or not ..
                if(!val){
                    while (getline(file, line) && !startWith(line, "$END"));
                }
            }
            else if (startWith(line, "$DTA"))
            {
                // Taking frame for the program....only for 10 INSTRUCTION
                getFrameForProgram();
                frame++; // incrementing frame size
                int totalInst = storeInstruction(inst);
                inst = "";
                // If error occurs then program terminate abnormally
                if(totalInst==-1){
                    while (getline(file, line) && !startWith(line, "$END"));
                    // start with next program .....
                    continue;
                } 
                cout << "Starting reading data....\n";
                while (getline(file, line) && !startWith(line, "$END"))
                {
                    int ind = 0, ch = 0, lastind;
                    cout << "Putting data into buffer\n";
                    buffer = vector<vector<char>>(10, vector<char>(4, '-'));
                    cout<<"Buffer length: "<<line.length()<<endl;
                    for (int i = 0; i < line.length(); i++)
                    {
                        cout<<"Data in buffer.."<<endl;
                        buffer[ind][ch] = line[i];
                        lastind = ind;
                        if (line[i] == ' ')
                        {
                            buffer[ind][ch] = '-';
                        }
                        ch++;
                        if (ch == 4)
                        {
                            ind++;
                            ch = 0;
                        }
                    }
                    if (lastind == ind)
                        ind += 1;
                    buffSize = ind;
                    numBuff++; //  adding number of buff
                    //Handling user Program error 
                    cout<<"Buffer look likes : "<<endl; 
                    for(int i=0;i<buffSize;i++){
                        for(int j=0;j<4;j++){
                            cout<<buffer[i][j]<<" ";
                        }cout<<endl;
                    }
                    if(!userProgram(totalInst)){
                       while (getline(file, line) && !startWith(line, "$END"));
                       break;
                    }
                }
                // check for out of data error .....
                if(numBuff<numGD){
                    Terminate(1,-1);
                }
            }
            else
            {
                // Appending All instructions....
                inst += line;
            }
        }
        file.close();
    }
    else
    {
        cout << "There is some unknown error\n";
    }

    return 0;
}