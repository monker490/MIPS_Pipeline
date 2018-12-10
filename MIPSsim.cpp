#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <bits/stdc++.h> 
#include <math.h>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;



int registers[32] = {0};
map<int, int> data;
map<int,string> instructAddress;
map<int, vector<int> > operands;
const int hi = 100;
const int lo = 101;
int MFLO,MFHI = 0;
stack<char> ls;
int waiting,executing,b5pc,divcounter,divpc;
int buf9pc,buf7pc,buf12pc,buf10pc;
int buf6, buf7[2], buf8, buf9[2], buf10[2], buf11, buf12,b7pc,b9pc,mempc,b12pc;
bool b7wb,b9wb,memwb,b12wb,wait,exec,stall,alu1active,alu2active,divactive,mulactive,divunit, b12, b11, b10, b9, b8, b7, b6, b5, b4, b3, b2 = false;
vector<int> buf1;
queue<int> buf2, buf3, buf4, buf5;
string init1[] = {"ADD", "SUB", "AND", "OR"};
string init2[] = {"SRL", "SRA", "ADDI", "ANDI", "ORI"};
string init3[] = {"MFHI","MFLO"};
set<string> ALU11 (init1,init1 + 4);
set<string> ALU12 (init2,init2 + 5);
set<string> ALU13 (init3,init3 + 2);
bool activeop[35] = {false};
bool swp = false;
bool curoperands[35] = {false};
bool curdestinations[35] = {false};
bool issuedoperands[35] = {false};
bool issuedestinations[35] = {false};


string findTwoscomplement(string str) 
{ 
    int n = str.length(); 
    int i; 
    for (i = n-1 ; i >= 0 ; i--) 
        if (str[i] == '1') 
            break; 
    if (i == -1) 
        return '1' + str; 
    for (int k = i-1 ; k >= 0; k--) 
    {  
        if (str[k] == '1') 
            str[k] = '0'; 
        else
            str[k] = '1'; 
    } 
    return str; 
}

long binary_to_number(string binaryString) {
    // bool flag = false;
    long value = 0;
    int indexCounter = 0;
    // if (binaryString.at(0) == '1') {
    //     binaryString = findTwoscomplement(binaryString);
    //     flag = true;
    // }
    
    for(int i=binaryString.length()-1;i>=0;i--) {
        if(binaryString[i]=='1') {
            value += pow(2, indexCounter);
        }
    indexCounter++;
    }
    return value;
}

string divprint() {
    ostringstream r,s;
    string temp = "[";
    r << buf7[0];
    temp += r.str();
    temp += ", ";
    s << buf7[1];
    temp += s.str();
    temp += "]";
    return temp;
}

string mulprint() {
    ostringstream s;
    string temp = "[";
    s << buf12;
    temp += s.str();
    temp += "]";
    return temp;
}

string alu1print() {
    ostringstream r,s;
    string temp = "[";
    r << buf9[0];
    temp += r.str();
    temp += ", R";
    s << buf9[1];
    temp += s.str();
    temp += "]";
    return temp;
}

string alu2print() {
    ostringstream r,s;
    string temp = "[";
    r << buf10[0];
    temp += r.str();
    temp += ", R";
    s << buf10[1];
    temp += s.str();
    temp += "]";
    return temp;
}

string decode(int addr) {
    // return instructAddress[addr];
    ostringstream r,s,t;
    string temp = "[";
    if (instructAddress[addr] == "ADD") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", R";
        temp += t.str();
    }
    if (instructAddress[addr] == "SUB") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", R";
        temp += t.str();
    }
    if (instructAddress[addr] == "AND") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", R";
        temp += t.str();
    }
    if (instructAddress[addr] == "OR") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", R";
        temp += t.str();
    }
    if (instructAddress[addr] == "SRL") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "SRA") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "ADDI") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "ANDI") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "ORI") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "MULT") {
        r << operands[addr][0];
        s << operands[addr][1];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
    }
    if (instructAddress[addr] == "DIV") {
        r << operands[addr][0];
        s << operands[addr][1];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
    }
    if (instructAddress[addr] == "MFHI") {
        r << operands[addr][0];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
    }
    if (instructAddress[addr] == "MFLO") {
        r << operands[addr][0]; 
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();        
    }
    if (instructAddress[addr] == "SW") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", ";
        temp += s.str();
        temp += "(R";
        temp += t.str();
        temp += ")";
    }
    if (instructAddress[addr] == "LW") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", ";
        temp += s.str();
        temp += "(R";
        temp += t.str();
        temp += ")";
    }
    if (instructAddress[addr] == "J") {
        r << operands[addr][0];
        temp += instructAddress[addr];
        temp += " #";
        temp += r.str();
    }
    if (instructAddress[addr] == "BEQ") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "BNE") {
        r << operands[addr][0];
        s << operands[addr][1];
        t << operands[addr][2];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", R";
        temp += s.str();
        temp += ", #";
        temp += t.str();
    }
    if (instructAddress[addr] == "BGTZ") {
        r << operands[addr][0];
        s << operands[addr][1];
        temp += instructAddress[addr];
        temp += " R";
        temp += r.str();
        temp += ", #";
        temp += s.str();
    }
    if (instructAddress[addr] == "BREAK") {
        temp += instructAddress[addr];
    }
    temp += "]";
    return temp;
} 


int main(int argc, char *argv[]) {
    
    string line;
    ifstream file(argv[1]);
    if (file.fail()) {
        cout <<"INCORRECT INPUT";
        exit(1);
    }
    
    ofstream disassembly;
    ofstream simulation;
    disassembly.open("disassembly.txt");
    simulation.open("simulation.txt");

    long prodiv;
    int c =1;
    int pc = 256;
    int ex = pc;
    long d,s1,s2,iv;
    int cycle = 0;
    int jump = 0;
    string category,opcode,instruction,dest,src1,src2,immediate_value,linetwo;
    bool flag = true;
    string p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25;
    bool activedest[35] = {false};
    

    while (getline(file,line)) {
        if (flag) {
            category = line.substr(0,3);
            opcode = line.substr(3,3);
            if (category == "000") { //CATEGORY 1
                if (opcode == "000") {
                    instruction = "J";
                    src1 = line.substr(6,string::npos);
                    s1 = binary_to_number(src1) * 4; //Left shifted by 2 bits
                    // cout << line << "\t" << pc << "\t" << instruction << " #" << s1 << "\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " #" << s1 << "\n";
                    operands[pc].push_back(s1);
                }
                else if (opcode == "001") {
                    instruction = "BEQ";
                    src1 = line.substr(6,5);
                    src2 = line.substr(11,5);
                    s1 = binary_to_number(src1);
                    s2 = binary_to_number(src2);
                    immediate_value = line.substr(16,string::npos); //offset
                    // iv = binary_to_number(immediate_value) * 4; //left shifted by 2 bits which is equivalent to multiply by 4
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value) * 4;
                    }
                    else {
                        iv = binary_to_number(immediate_value) * 4;
                    
                    }
                    // cout << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << ", #" << iv <<"\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << ", #" << iv <<"\n";
                    operands[pc].push_back(s1);
                    operands[pc].push_back(s2);
                    operands[pc].push_back(iv);
                }
                else if (opcode == "010") {
                    instruction = "BNE";
                    src1 = line.substr(6,5);
                    src2 = line.substr(11,5);
                    s1 = binary_to_number(src1);
                    s2 = binary_to_number(src2);
                    immediate_value = line.substr(16,string::npos); //offset
                    // iv = binary_to_number(immediate_value) * 4; //left shifted by 2 bits which is equivalent to multiply by 4
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value) * 4;
                    }
                    else {
                        iv = binary_to_number(immediate_value) * 4;
                    
                    }
                    // cout << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << ", #" << iv <<"\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << ", #" << iv <<"\n";
                    operands[pc].push_back(s1);
                    operands[pc].push_back(s2);
                    operands[pc].push_back(iv);
                }
                else if (opcode == "011") {
                    instruction = "BGTZ";
                    src1 = line.substr(6,5);
                    s1 = binary_to_number(src1);
                    immediate_value = line.substr(16,string::npos); //offset
                    // iv = binary_to_number(immediate_value) * 4; //left shifted by 2 bits which is equivalent to multiply by 4
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value) * 4;
                    }
                    else {
                        iv = binary_to_number(immediate_value) * 4;
                    
                    }
                    // cout << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", #" << iv <<"\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", #" << iv <<"\n";
                    operands[pc].push_back(s1);
                    operands[pc].push_back(iv);
                }
                else if (opcode == "100") {
                    instruction = "SW";
                    src1 = line.substr(6,5); //base
                    src2 = line.substr(11,5);
                    s1 = binary_to_number(src1); 
                    s2 = binary_to_number(src2);
                    immediate_value = line.substr(16,string::npos); //offset
                    // iv = binary_to_number(immediate_value);
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value);
                    }
                    else {
                        iv = binary_to_number(immediate_value);
                    
                    }
                    // cout << line << "\t" << pc << "\t" << instruction << " R" << s2 << ", " << iv << "(R" << s1 << ")" << "\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << s2 << ", " << iv << "(R" << s1 << ")" << "\n";
                    operands[pc].push_back(s2);
                    operands[pc].push_back(iv);
                    operands[pc].push_back(s1);
                    
                }
                else if (opcode == "101") {
                    instruction = "LW";
                    src1 = line.substr(6,5); //base
                    src2 = line.substr(11,5);
                    s1 = binary_to_number(src1); 
                    s2 = binary_to_number(src2);
                    immediate_value = line.substr(16,string::npos); //offset
                    // iv = binary_to_number(immediate_value);
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value);
                    }
                    else {
                        iv = binary_to_number(immediate_value);
                    
                    }
                    // cout << line << "\t" << pc << "\t" << instruction << " R" << s2 << ", " << iv << "(R" << s1 << ")" << "\n";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << s2 << ", " << iv << "(R" << s1 << ")" << "\n";
                    operands[pc].push_back(s2);
                    operands[pc].push_back(iv);
                    operands[pc].push_back(s1); 
                }
                else if (opcode == "110") {
                    instruction = "BREAK";
                    // cout << line << "\t" << pc << "\t" << "BREAK" << "\n";
                    disassembly << line << "\t" << pc << "\t" << "BREAK" << "\n";
                    flag = false;
                    operands[pc].push_back(0);
                    
                    
                }
                else {
                    instruction = "INCORRECT";
                }
            }
            else if (category == "001") { //CATEGORY 2
                dest = line.substr(6,5);
                src1 = line.substr(11,5);
                src2 = line.substr(16,5);
                d = binary_to_number(dest);
                s1 = binary_to_number(src1);
                
                s2 = binary_to_number(src2);
                if (opcode == "000") {
                    instruction = "ADD";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";
                }
                else if (opcode == "001") {
                    instruction = "SUB";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";
                }
                else if (opcode == "010") {
                    instruction = "AND";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";              
                }
                else if (opcode == "011") {
                    instruction = "OR";
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";
                }
                else if (opcode == "100") {
                    instruction = "SRL";
                    //s2 is SHIFT
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", #" << s2 <<"\n";    
                }
                else if (opcode == "101") {
                    instruction = "SRA";
                    //s2 is shift
                    disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", #" << s2 <<"\n";
                }
                else {
                    instruction = "INCORRECT";
                    instruction = "INCORRECT";
                    cout << line << "\t" << "instruction";
                    break;
                }
                // cout << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";
                // disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", R" << s2 <<"\n";
                operands[pc].push_back(d);
                operands[pc].push_back(s1);
                operands[pc].push_back(s2);
                
            }
            else if (category == "010") { //CATEGORY 3
                dest = line.substr(6,5);
                src1 = line.substr(11,5);
                immediate_value = line.substr(16,16);
                if (opcode == "000") {
                    instruction = "ADDI";
                    if (immediate_value.at(0) == '1'){
                        immediate_value = findTwoscomplement(immediate_value);
                        iv = -binary_to_number(immediate_value);
                    }
                    else {
                        iv = binary_to_number(immediate_value);
                    }
                }
                else if (opcode == "001") {
                    instruction = "ANDI";
                    iv = binary_to_number(immediate_value);
                }
                else if (opcode == "010") {
                    instruction = "ORI";
                    iv = binary_to_number(immediate_value);
                }
                else {
                    instruction = "INCORRECT";
                    cout << line << "\t" << "instruction";
                    break;
                }
                d = binary_to_number(dest);
                s1 = binary_to_number(src1);
                
                // cout << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", #" << iv <<"\n";
                disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << ", R" << s1 << ", #" << iv <<"\n";
                operands[pc].push_back(d);
                operands[pc].push_back(s1);
                operands[pc].push_back(iv);
            }
            else if (category == "011") { //CATEGORY 4
                src1 = line.substr(6,5);
                src2 = line.substr(11,5);
                s1 = binary_to_number(src1);
                s2 = binary_to_number(src2);
                if (opcode == "000") {
                    instruction = "MULT";
                }
                else if (opcode == "001") {
                    instruction = "DIV";
                }
                else {
                    instruction = "INCORRECT";
                    cout << line << "\t" << "instruction";
                    break;
                }
                // cout << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << "\n";
                disassembly << line << "\t" << pc << "\t" << instruction << " R" << s1 << ", R" << s2 << "\n";
                operands[pc].push_back(s1);
                operands[pc].push_back(s2);
                
            }
            else if (category == "100") { //CATEGORY 5
                dest = line.substr(6,5);
                if (opcode == "000") {
                    instruction = "MFHI";
                }
                else if (opcode == "001") {
                    instruction = "MFLO";
                }
                else {
                    instruction = "INCORRECT";
                    cout << line << "\t" << "instruction";
                    break;
                }
                d = binary_to_number(dest);
                // cout << line << "\t" << pc << "\t" << instruction << " R" << d << "\n";
                disassembly << line << "\t" << pc << "\t" << instruction << " R" << d << "\n";
                operands[pc].push_back(d);
            }
            instructAddress[pc] = instruction;
            // cout << instructAddress[pc] << "\t" <<  operands[pc][0] << "\n";
        }
        else {
            // if (line.substr(0,1) == "1") { //NEGATIVE NUMBERS
            //     dest = (bitset<32>(line) ^ bitset<32>(string("11111111111111111111111111111111"))).to_string();
            //     d = binary_to_number(dest) + 1;
            //     // cout << line << "\t" << pc << "\t" << "-" << d << "\n";
            //     disassembly << line << "\t" << pc << "\t" << "-" << d << "\n";
            //     data[pc] = -d;
            // }
            // else {
            //     d = binary_to_number(line);
            //     // cout << line << "\t" << pc << "\t" << d << "\n";
            //     // disassembly << line << "\t" << pc << "\t" << d << "\n";
            //     data[pc] = d;
            // }
            if (line.at(0) == '1'){
                linetwo = findTwoscomplement(line);
                d = -binary_to_number(linetwo);
                disassembly << line << "\t" << pc << "\t" <<  d << "\n";
                data[pc] = d;
            }
            else {
                d = binary_to_number(line);
                disassembly << line << "\t" << pc << "\t" << d << "\n";
                data[pc] = d;
            }
            
        }
        
         
        // disassembly << line<< "\t" << pc << "\n";
        pc = pc+4;
        d = s1 = s2  = iv = 0;
        dest = src1 = src2 = immediate_value = linetwo = "";
    }
    disassembly.close();
    
    flag = true;
    

    while (flag) {
        
        // -------------------WRITE BACK --------------------------
        
        if (b9wb) {
            if (find(ALU11.begin(),ALU11.end(),instructAddress[b9pc]) != ALU11.end()) { //ALGEBRAIC INSTRUCTIONS
                
                activedest[operands[b9pc][0]]= false;
                
                activeop[operands[b9pc][1]] = false;
                activeop[operands[b9pc][2]] = false;
                

            }
            if (find(ALU12.begin(),ALU12.end(),instructAddress[b9pc]) != ALU12.end()) { //IMMEDIATE INSTRUCTIONS
                
                activedest[operands[b9pc][0]]= false;
                activeop[operands[b9pc][1]] = false;
                
                    
            }
            else if (instructAddress[b9pc] == "MFHI") {
                
                activedest[operands[b9pc][0]]= false;
                activeop[32]= false;
            }
            else if (instructAddress[b9pc] == "MFLO") {
                activedest[operands[b9pc][0]]= false;
                activeop[33]= false;
            }                        
            b9wb = false;
        }
        if (memwb) {
            set<int>::iterator a,b;
            if (instructAddress[mempc] == "LW") {
                activedest[operands[mempc][0]]= false;
                activeop[operands[mempc][2]]= false;
            }
            else if (instructAddress[mempc] == "SW") {
                activeop[operands[mempc][0]]= false;
                activeop[operands[mempc][1]]= false;
            }
            
            
            memwb = false;
        }
        if (b12wb) {
            activedest[33] = false;
            activeop[operands[b12pc][0]] = false;
            activeop[operands[b12pc][1]] = false;
        }
        if (b7wb) {
            // if (instructAddress[b7pc])
            activedest[32] = false;
            activedest[33] = false;
            activeop[operands[b7pc][0]] = false;
            activeop[operands[b7pc][1]] = false;
            b7wb = false;
        }    
          
        //General convention is 0th element = value, 1st element = destination

        //ADD A WB UNIT??

        if (b9) { //Corresponds to ALU 1
            registers[buf9[1]] = buf9[0]; 
            b9 = false;
            b9pc = buf9pc;
            b9wb = true;
        }

        if (b10) { //Corresponds to ALU 2
            registers[buf10[1]] = buf10[0];
            mempc = buf10pc;
            b10 = false;
            memwb = true;
        }

        if (b12) { //Corresponds to MUL 
            MFLO = buf12;
            b12pc = buf12pc;
            b12 = false;
            b12wb = true;
            activedest[33] = true;
        }

        if (b7) { // Corresponds to DIV
            MFHI = buf7[0]; //remainder
            MFLO = buf7[1]; //quotient;
            b7pc = buf7pc;
            b7 = false;
            b7wb = true;
        }

        // ------------- ALU 1 --------------
        
        if (b5) {
            b5pc = buf5.front();
            buf9pc = b5pc;
            alu1active = true;
            if (instructAddress[b5pc] == "ADD") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] + registers[operands[b5pc][2]];
            }
            else if (instructAddress[b5pc] == "SUB") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] - registers[operands[b5pc][2]];
            }
            else if (instructAddress[b5pc] == "AND") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] & registers[operands[b5pc][2]];
            }
            else if (instructAddress[b5pc] == "OR") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] | registers[operands[b5pc][2]];
            }
            else if (instructAddress[b5pc] == "SRL") {
                buf9[1] = operands[b5pc][0];
                if (registers[operands[b5pc][1]] < 0) {
                    int n;
                    int counter = 0;
                    src2 = "";
                    src1 = findTwoscomplement(bitset<32>(-registers[operands[b5pc][1]]).to_string());
                    n = src1.length();
                    for (int i = n-1; i>=0 ;i--){
                        ls.push(src1[i]);
                    }
                    for (int i = 0; i < s2; i++){
                        ls.push('0');
                    }
                    while (!ls.empty())
                    {
                        //  cout << ' ' << l.top();
                        if (counter<32) {
                            src2+= ls.top();
                            counter ++;
                        }
                        ls.pop();
                    }
                    buf9[0] = binary_to_number(src2);           
                    src2 = src1 = "";
                }
                else {
                    buf9[0] = registers[operands[b5pc][1]] >> operands[b5pc][2];
                }
            }
            else if (instructAddress[b5pc] == "SRA") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] >> operands[b5pc][2];
            }
            else if (instructAddress[b5pc] == "ADDI") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] + operands[b5pc][2];
            }
            else if (instructAddress[b5pc] == "ANDI") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] & operands[b5pc][2];
            }
            else if (instructAddress[b5pc] == "ORI") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = registers[operands[b5pc][1]] | operands[b5pc][2];             
            }
            else if (instructAddress[b5pc] == "MFHI") {
                buf9[1] = operands[b5pc][0];
                buf9[0] = MFHI;
            }
            else if (instructAddress[b5pc] == "MFLO") {
                buf9[1] = operands[b5pc][0];                                                               
                buf9[0] = MFLO;
            }
            
            b9 = true;
        }


        //--------------- ALU 2 ----------- 

        if (b6) {
            
            if (instructAddress[buf6] == "LW") {
                buf10[1] = operands[buf6][0];
                buf10[0] = data[operands[buf6][1] + registers[operands[buf6][2]]];
                // cout << operands[ex][1] + registers[operands[ex][2]] << "\tasdasdljahdkjhasjdhakjdha\n";
                b10 = true;
                buf10pc = buf6;
            }
            else if (instructAddress[buf6] == "SW") {
                // cout << operands[ex][1] + registers[operands[ex][2]] << "\tasdasdljahdkjhasjdhakjdha\n";
                mempc = buf6;
                memwb = true;
                data[operands[buf6][1] + registers[operands[buf6][2]]] = registers[operands[buf6][0]];
                
            }
            b6 = false;
        }

        if (b2) {
            buf6 = buf2.front(); 
            alu2active = true;
             
            //deque the from buf 2 and pass it on to b6
            
            b6 = true;
        }

        //------------ MULTIPLICATION ------------

        if (b11) {
            b12 = true;
            buf12pc = buf11;
            // prodiv = product of previous multiplications
            buf12 = registers[operands[buf11][0]] * registers[operands[buf11][1]];
            b11 = false;
            activedest[33] = true;
        }

        if (b8) {
            b11 = true;
            buf11 = buf8;
            // Move along the multiplication pipeline
            b8 = false;
            activedest[33] = true;
        }

        if (b4) {
            buf8 = buf4.front();
            mulactive = true;
            // buf4.pop();
            b8 = true;
            activedest[33] = true;
            // mov the multiplication forward
             
        }

        // --------------- DIVISION ----------------

        if (divunit) {        
            if (divcounter == 0) {
                buf7pc = divpc;
                buf7[0] = registers[operands[divpc][0]]%registers[operands[divpc][1]];
                buf7[1] = registers[operands[divpc][0]]/registers[operands[divpc][1]]; 
                b7 = true;
                divunit = false;
                }
            else {
                divcounter --;
            }
        }

        if (b3) {
            if (!divunit) {
                divpc = buf3.front();
                divactive = true;
                
                divcounter = 3;
                divunit = true;
            }
            
        }

        // -------------------- ISSUE ------------------
        // FOR RAW CHECK IF OPERANDS NOT IN DESTINATIONS

        // FOR WAR CHECK IF CURRENT DESTINATION IS AN OPERAND FOR PREVIOUS UNISSUED INSTRUCTIONS

        // && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false

        for(int i = 0; i<34; i++){
            curdestinations[i] = false;
            curoperands[i] = false;
            issuedoperands[i] = false;
            issuedestinations[i] = false;
        }
        swp = false;
        // && currdestinations[operands[*i][1] == false && currdestinations[operands[*i][2]] == false
        for(vector<int>::iterator i = buf1.begin(); i != buf1.end(); ++i) {
            
            if (find(ALU11.begin(),ALU11.end(),instructAddress[*i]) != ALU11.end()) { //ALGEBRAIC INSTRUCTIONS
                if (buf5.size() < 2 && activedest[operands[*i][1]] == false && activedest[operands[*i][2]] == false && curoperands[operands[*i][0]] == false && issuedoperands[operands[*i][0]] == false && curdestinations[operands[*i][1]] == false && curdestinations[operands[*i][2]] == false && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false){
                        // cout << *i;
                        buf5.push(*i);
                        activedest[operands[*i][0]] = true;
                        activeop[operands[*i][1]] = true;
                        activeop[operands[*i][2]] = true;
                        issuedestinations[operands[*i][0]] = true;
                        issuedoperands[operands[*i][1]] = true;
                        issuedoperands[operands[*i][2]] = true;
                        buf1.erase(i);
                        i--;
                        b5 = true;
                        continue;
                    }
                    else {
                        curdestinations[operands[*i][0]] = true;
                        curoperands[operands[*i][1]] = true;
                        curoperands[operands[*i][2]] = true;
                    }
                
            }
            else if (find(ALU12.begin(),ALU12.end(),instructAddress[*i]) != ALU12.end()) { //IMMEDIATE INSTRUCTIONS
                if (buf5.size() < 2 && activedest[operands[*i][1]] == false && curoperands[operands[*i][0]] == false && issuedoperands[operands[*i][0]] == false && curdestinations[operands[*i][1]] == false && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false) {
                        // cout << *i;
                        buf5.push(*i);
                        activedest[operands[*i][0]] = true;
                        activeop[operands[*i][1]] = true;
                        issuedestinations[operands[*i][0]] = true;
                        issuedoperands[operands[*i][1]] = true;
                        buf1.erase(i);
                        i--;
                        b5 = true;
                        continue;
                    }
                    else {
                        curdestinations[operands[*i][0]] = true;
                        curoperands[operands[*i][1]] = true;
                    }
                
            }
            // else if (find(ALU13.begin(),ALU13.end(),instructAddress[*i]) != ALU13.end()) { //MFHI MFLO
            else if (instructAddress[*i] == "MFHI") {
                    if (buf5.size() < 2 && activedest[32] == false && curoperands[operands[*i][0]] == false && issuedoperands[operands[*i][0]] == false && curdestinations[32] == false && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false) { // 32 corresponds to HI
                            buf5.push(*i);
                            activedest[operands[*i][0]] = true;
                            activeop[32] = true;
                            issuedestinations[operands[*i][0]] = true;
                            issuedoperands[32] = true;
                            buf1.erase(i);
                            i--;
                            b5 = true;
                            continue;
                        }
                        else {
                            curdestinations[operands[*i][0]] == true;
                            curoperands[32] = true;
                        }
                
            }
            else if (instructAddress[*i] == "MFLO") {
                    if (buf5.size() < 2 && activedest[33] == false && curoperands[operands[*i][0]] == false && issuedoperands[operands[*i][0]] == false && curdestinations[33] == false && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false) { // 33 corresponds to LO 
                            // cout << activedest[33];
                            // cout << curdestinations[33];
                            // cout << c;
                            // cin >> dest;
                            buf5.push(*i);
                            activedest[operands[*i][0]] = true;
                            activeop[33] = true;
                            issuedestinations[operands[*i][0]] = true;
                            issuedoperands[33] = true;
                            buf1.erase(i);
                            i--;
                            b5 = true;
                            continue;
                            
                        }
                        else {
                            curdestinations[operands[*i][0]] = true;
                            curoperands[33] = true;
                            // cout << operands[*i][0] << "Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
                        }
                
            }
            else if (instructAddress[*i] == "MULT") {
                if (buf4.size() < 2 && activedest[operands[*i][0]] == false && activedest[operands[*i][1]] == false && curoperands[33] == false && issuedoperands[33] == false && curdestinations[operands[*i][0]] == false && curdestinations[operands[*i][1]] == false && curdestinations[33] == false && activedest[33] == false && issuedestinations[33] == false) {
                        activedest[33] = true;
                        activeop[operands[*i][0]] = true;
                        activeop[operands[*i][1]] = true;
                        issuedestinations[33] = true;
                        issuedoperands[operands[*i][0]] = true;
                        issuedoperands[operands[*i][1]] = true;                        
                        buf4.push(*i);
                        buf1.erase(i);
                        i--;
                        b4 = true;
                        // cout << activedest[33];
                        // cout << curdestinations[33];
                        // cout << c;
                        continue;
                    }
                    else {
                        curdestinations[33] = true;
                        curoperands[operands[*i][0]] = true;
                        curoperands[operands[*i][1]] = true;                        
                    }
                
            }
            else if (instructAddress[*i] == "DIV") {
                if (buf3.size() < 2 && activedest[operands[*i][0]] == false && activedest[operands[*i][1]] == false && curoperands[32] == false && curoperands[33] == false && issuedoperands[32] == false && issuedoperands[33] == false && curdestinations[operands[*i][0]] == false && curdestinations[operands[*i][1]] == false && curdestinations[32] == false && activedest[32] == false && issuedestinations[32] == false && curdestinations[33] == false && activedest[33] == false && issuedestinations[33] == false) {
                        activedest[32] = true;
                        activedest[33] = true;
                        activeop[operands[*i][0]]=true;
                        activeop[operands[*i][1]] = true;
                        issuedestinations[32] = true;
                        issuedestinations[33] = true;
                        issuedoperands[operands[*i][0]] = true;
                        issuedoperands[operands[*i][1]] = true;
                        buf3.push(*i);
                        buf1.erase(i);
                        i--;
                        b3 = true;
                        continue;
                    }
                    else {
                        curdestinations[32] = true;;
                        curdestinations[33] = true;;
                        curoperands[operands[*i][0]]=true;
                        curoperands[operands[*i][1]] = true;
                    }
                
            }
            else if (instructAddress[*i] == "LW") {
                for (vector<int>::iterator x=buf1.begin();x!=i;x++) {
                    if (instructAddress[*x] == "SW"){
                        swp = true;
                    }
                }
                if (buf2.size() < 2 && activedest[operands[*i][2]] == false && curoperands[operands[*i][0]] == false && issuedoperands[operands[*i][0]] == false && curdestinations[operands[*i][2]] == false && curdestinations[operands[*i][0]] == false && activedest[operands[*i][0]] == false && issuedestinations[operands[*i][0]] == false && swp == false) {
                    activedest[operands[*i][0]] = true;
                    activeop[operands[*i][2]]= true;
                    issuedestinations[operands[*i][0]] = true;
                    issuedoperands[operands[*i][2]] = true;
                    buf2.push(*i);
                    buf1.erase(i);
                    i--;
                    b2 = true;
                    continue;
                }
                else {
                    curdestinations[operands[*i][0]] = true;
                    curoperands[operands[*i][2]]= true;
                }
                
            }
            else if (instructAddress[*i] == "SW") {
                for (vector<int>::iterator x=buf1.begin();x!=i;x++) {
                    if (instructAddress[*x] == "SW"){
                        swp = true;
                    }
                }
                if (buf2.size() < 2 && activedest[operands[*i][0]] == false && activedest[operands[*i][1]] == false && curdestinations[operands[*i][0]] == false && curdestinations[operands[*i][1]]== false && swp == false) {
                    activeop[operands[*i][0]] = true;
                    activeop[operands[*i][2]] = true;
                    issuedoperands[operands[*i][0]] = true;
                    issuedoperands[operands[*i][2]] = true;
                        
                    buf2.push(*i);
                    buf1.erase(i);
                    i--;
                    b2 = true;
                    continue;
                }
                else {
                    curoperands[operands[*i][0]] = true;
                    curoperands[operands[*i][2]] = true;
                }
            }      

        }
        
        
        
        
        // --------------------- FETCH -----------

        
        if (exec) {
            exec = false;
            stall = false;
            if (instructAddress[executing] == "BEQ") {
                registers[operands[executing][0]] == registers[operands[executing][1]] ? ex = executing + operands[executing][2] + 4: ex = executing + 4; 
            }
            if (instructAddress[executing] == "BNE") {
                registers[operands[executing][0]] != registers[operands[executing][1]] ? ex = executing + operands[executing][2] + 4: ex = executing + 4; 
            }
            if (instructAddress[executing] == "BGTZ") {
                registers[operands[executing][0]] > 0 ? ex = executing + operands[executing][1] + 4: ex = executing + 4;
            }
            if (instructAddress[executing] == "J") {
                ex = operands[executing][0];
            }
            if (instructAddress[executing] == "BREAK") {
                flag = false;
            }
        }
        
        
        if (wait) {
            
            if (instructAddress[waiting] == "BGTZ") { 
                if (curdestinations[operands[waiting][0]] == false && activedest[operands[waiting][0]] == false) {
                    // cout << operands[waiting][0];
                for (int i=0; i<34;i++) {
                    // cout <<"\n"<<i<<"\t"<<curdestinations[i]<<"\t"<<activedest[i];
                }
                exec = true;
                executing = waiting;
                wait = false;        
                }        
            }
            if (instructAddress[waiting] == "BNE") { 
                if (curdestinations[operands[waiting][0]] == false && activedest[operands[waiting][0]] == false && curdestinations[operands[waiting][1]] == false && activedest[operands[waiting][1]] == false) {
                exec = true;
                executing = waiting;
                wait = false;        
                }        
            }
            if (instructAddress[waiting] == "BEQ") { 
                if (curdestinations[operands[waiting][0]] == false && activedest[operands[waiting][0]] == false && curdestinations[operands[waiting][1]] == false && activedest[operands[waiting][1]] == false) {
                exec = true;
                executing = waiting;
                wait = false;        
                }        
            }
            //if no hazards remain
        }
        
        
        
        
        
        for (int i =0; i<4; i++) {
            if (buf1.size() < 8 && stall == false) {
                
                if (instructAddress[ex] == "BEQ" || instructAddress[ex] == "BNE" || instructAddress[ex] == "BGTZ") {
                    stall = true;
                    wait = true;
                    waiting = ex;

                }
                else if (instructAddress[ex] == "J") {
                    exec = true;
                    stall = true;
                    executing = ex;
                }
                else if (instructAddress[ex] == "BREAK"){
                    flag = false;
                    exec = true;
                    stall = false;
                    executing = ex;
                }
                else {
                buf1.push_back(ex);
                ex = ex + 4;
                }   
            } 
        }

        if (alu1active) {
            buf5.pop();
            alu1active = false;
            if (buf5.empty()) {
                b5 = false;
            }
        }
        if (alu2active) {
            buf2.pop();
            alu2active = false;
            if (buf2.empty()) {
                b2 = false;
            }
        }
        if (mulactive) {
            buf4.pop();
            mulactive = false;
            if (buf4.empty()) {
                b4 = false;
            }
        }
        if (divactive) {
            buf3.pop();
            divactive = false;
            if (buf3.empty()) {
                 b3 = false;
            }
        }
        

        // PUT WRITEBACK HERE


        // cin >> dest;
        // cout <<"fetch done-----------------------------------------------------------------";
        
        // ---------------- PRINTING ----------------------
        p1 = ((wait) ? decode(waiting) : "");
        p2 = ((exec) ? decode(executing) : "");
        p3 = ((buf1.size()>=1) ? decode(buf1.at(0)) : "");
        p4 = ((buf1.size()>=2) ? decode(buf1.at(1)) : "");
        p5 = ((buf1.size()>=3) ? decode(buf1.at(2)) : "");
        p6 = ((buf1.size()>=4) ? decode(buf1.at(3)) : "");
        p7 = ((buf1.size()>=5) ? decode(buf1.at(4)) : "");
        p8 = ((buf1.size()>=6) ? decode(buf1.at(5)) : "");
        p9 = ((buf1.size()>=7) ? decode(buf1.at(6)) : "");
        p10 = ((buf1.size()>=8) ? decode(buf1.at(7)) : "");
        p11 = ((buf2.size()>=1) ? decode(buf2.front()) : "");
        p12 = ((buf2.size()>=2) ? decode(buf2.back()) : "");
        p13 = ((buf3.size()>=1) ? decode(buf3.front()) : "");
        p14 = ((buf3.size()>=2) ? decode(buf3.back()) : "");
        p15 = ((buf4.size()>=1) ? decode(buf4.front()) : "");
        p16 = ((buf4.size()>=2) ? decode(buf4.back()) : "");
        p17 = ((buf5.size()>=1) ? decode(buf5.front()) : "");
        
        p18 = ((buf5.size()>=2) ? decode(buf5.back()) : "");
        p19 = ((b6) ? decode(buf6) : "");
        p20 = ((b7) ? divprint() : "");
        p21 = ((b8) ? decode(buf8) : "");
        p22 = ((b9) ? alu1print() : "");
        p23 = ((b10) ? alu2print() : "");
        p24 = ((b11) ? decode(buf11) : "");
        p25 = ((b12) ? mulprint() : "");

        simulation << "--------------------" << "\n";
        simulation << "Cycle " << c << ":\n\n";
        simulation << "IF:\n";
        simulation << "\tWaiting: "<< p1 << "\n";
        simulation << "\tExecuted: "<< p2<< "\n";
        simulation << "Buf1:" << "\n";
        simulation << "\tEntry 0: "<< p3 << "\n";
        simulation << "\tEntry 1: "<< p4<< "\n";
        simulation << "\tEntry 2: "<< p5<< "\n";
        simulation << "\tEntry 3: "<< p6<< "\n";
        simulation << "\tEntry 4: "<< p7<< "\n";
        simulation << "\tEntry 5: "<< p8<< "\n";
        simulation << "\tEntry 6: "<< p9<< "\n";
        simulation << "\tEntry 7: "<< p10<< "\n";
        simulation << "Buf2:" << "\n";
        simulation << "\tEntry 0: "<< p11<< "\n";
        simulation << "\tEntry 1: "<< p12<< "\n";
        simulation << "Buf3:" << "\n";
        simulation << "\tEntry 0: "<< p13<< "\n";
        simulation << "\tEntry 1: "<< p14<< "\n";
        simulation << "Buf4:" << "\n";
        simulation << "\tEntry 0: "<< p15<< "\n";
        simulation << "\tEntry 1: "<< p16<< "\n";
        simulation << "Buf5:" << "\n";
        simulation << "\tEntry 0: "<< p17<< "\n";
        simulation << "\tEntry 1: "<< p18<< "\n";
        simulation << "Buf6:" << p19 << "\n";
        simulation << "Buf7:" << p20 << "\n";
        simulation << "Buf8:" << p21 << "\n";
        simulation << "Buf9:" << p22 << "\n";
        simulation << "Buf10:" << p23 << "\n";
        simulation << "Buf11:" << p24 << "\n";
        simulation << "Buf12:" << p25 << "\n";        

        simulation << "\nRegisters\n";
        simulation << "R00:\t" << registers[0] << "\t" << registers[1] << "\t" << registers[2] << "\t" << registers[3] << "\t" << registers[4] << "\t" << registers[5] << "\t" << registers[6] << "\t" << registers[7] << "\n";
        simulation << "R08:\t" << registers[8] << "\t" << registers[9] << "\t" << registers[10] << "\t" << registers[11] << "\t" << registers[12] << "\t" << registers[13] << "\t" << registers[14] << "\t" << registers[15] << "\n";
        simulation << "R16:\t" << registers[16] << "\t" << registers[17] << "\t" << registers[18] << "\t" << registers[19] << "\t" << registers[20] << "\t" << registers[21] << "\t" << registers[22] << "\t" << registers[23] << "\n";
        simulation << "R24:\t" << registers[24] << "\t" << registers[25] << "\t" << registers[26] << "\t" << registers[27] << "\t" << registers[28] << "\t" << registers[29] << "\t" << registers[30] << "\t" << registers[31] << "\n";
        simulation << "HI: " << MFHI << "\n";
        simulation << "LO: " << MFLO << "\n";
        simulation << "\nData";
        {
            int i = 0;
            for(map<int,int>::iterator iter = data.begin(); iter != data.end(); ++iter){
                if (i%8 == 0) {
                    // cout<< "\n" << iter->first <<":";
                    simulation<< "\n" << iter->first <<":";
                }
                // cout << "\t" << iter->second;
                simulation << "\t" << iter->second;
                i++;
            }
            // cout << "\n";
            simulation << "\n";
        }
        c++; 
               

    }
    simulation.close();
}



