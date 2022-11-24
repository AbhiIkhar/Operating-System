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
#include <cstdlib> // for Random Number generation
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
#include "PCB.hpp"
PCB pcb;
// Setting interrupt
int SI = 3, TI = 0, PI;
// ------------------------------------------------------------
/// @brief GD
/// @param address
void evalGD(int add)
{
    cout << "Evaluating GD....\n";
    int n = buffSize;
    cout << "Buffer size: " << n << endl;
    for (int k = 0; k < n; k++)
    {
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
                    count++;
                    if (count == 4)
                    {
                        break;
                    }
                    continue;
                }
                l += M[i][k];
            }
            if (count == 4)
            {
                break;
            }
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
/// @brief Mater Mode
void MOS(int address)
{
    cout << "In the MOS...\n";
    switch (SI)
    {
    case 1:
        evalGD(address);
        break;
    case 2:
        evalPD(address);
        break;
    case 3:
        evalH();
        break;
    default:
        break;
    }
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
void userProgram(int totalInst)
{
    cout << "Inside userProgram" << endl;
    // TODO: Change this for Phase-2
    bool nextBuffer = false;
    while (IC < totalInst)
    {
        IR = M[IC];
        if (IR[0] == 'H')
        {
            SI = 3;
            MOS(-1);
            break;
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            if (nextBuffer)
            {
                break;
            }
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            SI = 1; // setting interrupt
            MOS(add);
            nextBuffer = true;
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            SI = 2; // setting interrupt
            MOS(add);
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            loadRegister(add);
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            storeRegister(add);
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            compareRegister(add);
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            branchOnto(add);
        }
        IC++;
    }
}
/// @brief Program card
int storeInstruction(string inst)
{
    cout << "Inside storeInstruction() " << endl;
    int add = 0, inn = 0, n = inst.size();
    for (int i = 0; i < n; i++)
    {
        if (inst[i] == 'H')
        {
            M[add][inn] = inst[i];
            inn = 0;
            add++;
            continue;
        }
        M[add][inn] = inst[i];
        inn++;
        if (inn == 4)
        {
            
            add++;
            inn = 0;
        }
    }
    int totalInst = add;
    cout << "After store instruction() memory : " << endl;
    add = 0, inn = 0;
    for (int i = 0; i < n; i++)
    {
        if (inst[i] == 'H')
        {
            cout << M[add][inn] << endl;
            inn = 0;
            add++;
            continue;
        }
        for (int j = 0; j <= 3; j++)
        {
            cout << M[add][j] << " ";
            i++;
        }
        cout << endl;
        add++;
        i--;
    }
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
    // rand() generate pseudo-random number in the range of [0, RAND_MAX)
    int num = rand() % 30;
    return num;
}
/// @brief its mapping the virtual address--> real address
/// @param VA
int addressMap(int VA)
{
    int PTE = PTR + VA/10;
    int add = M[PTE][2]*10 + M[PTE][3];
    int RA = (add)*10 + VA%10;
    return RA;
}
/// @brief Append error
/// @param errorNum 
/// @param line 
string appendErrorMess(int errorNum,string line){
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
        file2.close();
    }
}
// ------------------------------------------------------------

/// @brief Page table is creating
void createPageTable(){
    cout<<"Inside createTable() "<<endl;
    int val = PTR*10;
    for(int i=val;i<val+10;i++){
        for(int j=0;j<4;j++){
            M[i][j] = '0';
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
    buffer = vector<vector<char>>(10, vector<char>(4, '-'));
    buffSize = 0;
    // Phase-2 stuff below...
    // initializing pcb
    pcb.setJID(id);
    pcb.setTTL(tl);
    pcb.setTLL(ll);
    cout<<"JOBID : "<<pcb.getJID()<<endl;
    cout<<"TTL : "<<pcb.getTTL()<<endl;
    cout<<"TLL : "<<pcb.getTLL()<<endl;
    // initializing ptr and creating page table;
    PTR = ALLOCATE();
    createPageTable();
    cout<<"PTR : "<<PTR<<endl;
}
int excuteAMJ(string line)
{
    cout<<"Inside excuteAMJ"<<endl;
    int n = line.length();
    if (n != 16)
    {
        cout << "Your contol card is invalid..." << endl;
        return 0;
    }
    int id = 0, tl = 0, ll = 0, i = 4;
    int count = 4; // to keep track of 4 element
    while (count--)
    {
        id = id * 10 + (line[i] - '0');
        i++;
        // here instead of int any char can be present;
        // TODO--- HANDLE THAT ERROR ALSO
    }
    count = 4;
    while (count--)
    {
        tl = tl * 10 + (line[i] - '0');
        i++;
        // here instead of int any char can be present;
        // TODO--- HANDLE THAT ERROR ALSO
    }
    count = 4;
    while (count--)
    {
        ll = ll * 10 + (line[i] - '0');
        i++;
        // here instead of int any char can be present;
        // TODO--- HANDLE THAT ERROR ALSO
    }
    //calling INIT function 
    INIT(id,tl,ll);
    return 1;
}
void getFrameForProgram(){
    cout<<"Inside getFrameForProgram()"<<endl;
    int val=ALLOCATE();
    while(val==PTR){
        val=ALLOCATE();
    }
    cout<<"Frame No is :"<<val<<endl;
    M[PTR*10][2] = (val/10 + '0');
    M[PTR*10][3] = (val%10 + '0');
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
            cout << line << endl;
            if (startWith(line, "$AMJ"))
            {
                int val = excuteAMJ(line);
                if(!val){
                    while (getline(file, line) && !startWith(line, "$END"));
                }
            }
            else if (startWith(line, "$DTA"))
            {
                // Taking frame for the program....
                getFrameForProgram();
                int totalInst = storeInstruction(inst);
                inst = "";
                cout << "Starting reading data....\n";
                while (getline(file, line) && !startWith(line, "$END"))
                {
                    int ind = 0, ch = 0, lastind;
                    cout << "Putting data into buffer\n";
                    buffer = vector<vector<char>>(10, vector<char>(4, '-'));
                    for (int i = 0; i < line.length(); i++)
                    {
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
                    userProgram(totalInst);
                }
            }
            else
            {
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