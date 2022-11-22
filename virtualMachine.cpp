/*
M: memory- *
IR: Instruction Register -*
C: Instruction Counter Register -*
R:General Purpose Register -*
C:Toggle-*
Buffer-*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

/// @brief Control card -- $AMJ
void INIT()
{
    cout << "Inside INIT()" << endl;
    M = vector<vector<char>>(100, vector<char>(4, '-'));
    IR = vector<char>(4, '-');
    R = vector<char>(4, '-');
    IC = 0;
    C = false;
    buffer = vector<vector<char>>(10, vector<char>(4, '-'));
    buffSize = 0;
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
        for (int i = 0; i <= 3; i++)
        {
            M[add + k][i] = buffer[k][i];
            // cout << "Buffer[" << k << "][" << i << "]"
            //      << ": " << buffer[k][i] << endl;
        }
    }
    cout << "Whole memory after GD: " << endl;
    for (int i = 0; i < 100; i++)
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
                    if(count==4){
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
    cout << "Evaluating PD....\n";
    ofstream file2("linePrinter.txt", ios::app);
    file2 << "\n";
    file2 << "\n";
    file2.close();
}
/// @brief Mater Mode
void MOS(int SI, int address)
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
    bool nextBuffer = false;
    while (IC < totalInst)
    {
        IR = M[IC];
        if (IR[0] == 'H')
        {
            MOS(3, -1);
            break;
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            if (nextBuffer)
            {
                break;
            }
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            MOS(1, add);
            nextBuffer = true;
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            int add = (IR[2] - '0') * 10 + (IR[3] - '0');
            MOS(2, add);
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
                INIT();
            }
            else if (startWith(line, "$DTA"))
            {
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
        cout << "There is some Error\n";
    }

    return 0;
}