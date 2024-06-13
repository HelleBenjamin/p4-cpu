#include <iostream> // include all the necessary libraries
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>

#define helpMessage "-c <source file> -o <output file> -h help -v version -BIN binary output -HEX hex output -SYNTAX=<SYNTAX>"
#define supportedArchitectures "Supported architectures: P4"
#define version "Pepe's assembler v1.1"

using namespace std;

/* 
---P4--
Codename: PCPU-1004, 1st generation

P4 instruction set(Legacy Syntax):
0 - NOP (No operation)
1 - LDA (Load A with imm)
2 - ADD (Add A + imm -> OR)
3 - JMP (Unconditional jump)

P4 instruction set(Modern Syntax):
0 - NOP (No operation)
1 - MOVA (Imm -> A)
2 - ADD (Add A + imm -> OR)
3 - JP (Unconditional jump)

Architecture: P4

Instruction byte:
2-bit empty, 4-bit immediate, 2-bit instruction

Addressing modes:
- Immediate

*/


string program[16]; // source program
string assembledProgramBIN[16];
string assembledProgramHEX[16];

bool format = false; // 0 - binary, 1 - hex
bool syntax = false; // 0 - Legacy, 1 - Modern

string filename;
ifstream source_file;
ofstream output_file;

string dectohex(int num) {
    stringstream ss;
    ss << hex << num;
    return ss.str();
}

void loadProgram(){
    for (int i = 0; i < 15; i++) {
        assembledProgramBIN[i] = "00000000";
    }
    for (int i = 0; i < 15 && getline(source_file, program[i]); i++);
}


class assembler {
public:
    void p4() {
        uint8_t startAddress = 0;
        loadProgram();
        if (program[0].find(".org") != string::npos) {
            startAddress = stoi(program[0].substr(4));
            assembledProgramBIN[0] = bitset<6>(startAddress).to_string() + "11";
            for (int i = 1; i < startAddress; i++) {
                assembledProgramBIN[i] = "00000000";
            }
        }
        for (int i = 0; i < 15; i++) {
            if (program[i].find(';') == 0) {
                for(int j = i; j < 14; j++) {
                    program[j] = program[j + 1];
                }
            }
        }
        switch (syntax) {
        case 0:
            for (int i = 0; i < 15 ; i++) {
                string instruction = program[i];
                if (instruction.empty()) continue;
                if (instruction.find("nop") != string::npos) {
                    assembledProgramBIN[i + startAddress] = "00000000";
                } else if (instruction.find("lda ") != string::npos) {
                    string imm = instruction.substr(4);
                    string binaryImm = bitset<6>(stoi(imm)).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "01";
                } else if (instruction.find("add ") != string::npos) {
                    string imm = instruction.substr(4);
                    string binaryImm = bitset<6>(stoi(imm)).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "10";
                } else if (instruction.find("jmp ") != string::npos) {
                    string binaryImm = bitset<6>(stoi(instruction.substr(4))).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "11";
                }
            }
            break;
        case 1:
            for (int i = 0; i < 15 ; i++) {
                string instruction = program[i];
                if (instruction.empty()) continue;
                if (instruction.find("nop") != string::npos) {
                    assembledProgramBIN[i + startAddress] = "00000000";
                } else if (instruction.find("mova ") != string::npos) {
                    string imm = instruction.substr(5);
                    string binaryImm = bitset<6>(stoi(imm)).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "01";
                } else if (instruction.find("add ") != string::npos) {
                    string imm = instruction.substr(4);
                    string binaryImm = bitset<6>(stoi(imm)).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "10";
                } else if (instruction.find("jp ") != string::npos) {
                    string binaryImm = bitset<6>(stoi(instruction.substr(3))).to_string();
                    assembledProgramBIN[i + startAddress] = binaryImm + "11";
                }
            }
            break;
        default:
            break;
        }
        
        for (int i = 0; i < 15; i++) {
            if (format == 0) {
                output_file << assembledProgramBIN[i] << endl;
                cout << i << " " << assembledProgramBIN[i] << endl;
            } else {
                output_file << dectohex(stoi(assembledProgramBIN[i], nullptr, 2)) << endl;
                cout << i << " " << dectohex(stoi(assembledProgramBIN[i], nullptr, 2)) << endl;
            }
        }       
    }
};


int main(int argc, char *argv[]) {
    assembler Assembler;
    if (argc == 1) {
        cout << "Error: No arguments" << endl;
        cout << "Usage: " << helpMessage << endl;
        return 1;
    }
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-h") {
            cout << helpMessage << endl;
        }
        if ((string(argv[i])).find("-c") == 0) {
            filename = argv[i + 1];
            source_file.open(filename);
        }
        if ((string(argv[i])).find("-o") == 0) {
            filename = argv[i + 1];
            output_file.open(filename);
        }
        if (string(argv[i]) == "-v") {
            cout << version << endl;
            cout << supportedArchitectures << endl;
        }
        if (string(argv[i]) == "-BIN") {
            format = 0;
        }
        if (string(argv[i]) == "-HEX") {
            format = 1;
        }
        if (string(argv[i]) == "-SYNTAX=LEGACY") {
            syntax = 0;
        }
        if (string(argv[i]) == "-SYNTAX=MODERN") {
            syntax = 1;
        }
    }
    if (source_file.is_open() && output_file.is_open()) {
        Assembler.p4();
        source_file.close();
        output_file.close();
    }
    return 0;
}
