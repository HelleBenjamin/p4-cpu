#include <iostream> // include all the necessary libraries
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>

#define helpMessage "-c <source file> -o <output file> -h help -v version -BIN binary output -HEX hex output"
#define supportedArchitectures "Supported architectures: P4"
#define version "Pepe's assembler v1.0"

using namespace std;

vector<string> program; // source program
vector<string> assembledProgramBIN; //binary program
vector<string> assembledProgramHEX; //hex program

bool format = false; // 0 - binary, 1 - hex

/* Architectures:
 - P4 4-bit Cpu
 
P4 instruction set:
0 - NOP (No operation)
1 - LDA (Load A with imm)
2 - ADD (Add A + imm -> OR)
3 - JMP (Unconditional jump)

 Arguments:
 -c <source file>
 -o <output file>
 -h help
 -v version
 -BIN binary output
 -HEX hex output
*/

string filename;
ifstream source_file;
ofstream output_file;

string dectohex(int num) {
    stringstream ss;
    ss << hex << num;
    return ss.str();
}

void p4() {
    string line;
    while(getline(source_file, line)) {
        program.push_back(line);
    }
    for(int i = 0; i < program.size(); i++) {
        if(program[i].find("nop") == 0 | program[i].find("NOP") == 0) {
            assembledProgramBIN.push_back("00000000");
        }
        if(program[i].find("lda ") == 0 | program[i].find("LDA") == 0) {
            int imm = stoi(program[i].substr(4));
            string binaryImm = bitset<6>(imm).to_string();
            assembledProgramBIN.push_back(binaryImm+"01");
        }
        if(program[i].find("add ") == 0 | program[i].find("ADD") == 0) {
            int imm = stoi(program[i].substr(4));
            string binaryImm = bitset<6>(imm).to_string();
            assembledProgramBIN.push_back(binaryImm+"10");
        }
        if(program[i].find("jmp ") == 0 | program[i].find("JMP") == 0) {
            int imm = stoi(program[i].substr(4));
            string binaryImm = bitset<6>(imm).to_string();
            assembledProgramBIN.push_back(binaryImm+"11");
        }
    }
    for(int i = 0; i < assembledProgramBIN.size(); i++) {
        if(format == 0) {
            output_file << assembledProgramBIN[i] << endl;
            cout << assembledProgramBIN[i] << endl;
        } else if(format == 1) {
            output_file << dectohex(stoi(assembledProgramBIN[i], nullptr, 2)) << endl;
            cout << dectohex(stoi(assembledProgramBIN[i], nullptr, 2)) << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    for(int i = 0; i < argc; i++) {
        if (argc == 1) {
            cout << "Error: No arguments" << endl;
            cout << "Usage: ";
            cout << helpMessage << endl;
            return 1;
        }   
        if(string(argv[i]) == "-h") {
            cout << helpMessage << endl;
        }
        if((string(argv[i])).find("-c") == 0) {
            filename = argv[i+1];
            source_file.open(filename);
        }
        if((string(argv[i])).find("-o") == 0) {
            filename = argv[i+1];
            output_file.open(filename);
        }
        if(string(argv[i]) == "-v") {
            cout << version << endl;
            cout << supportedArchitectures << endl;
        }
        if(string(argv[i]) == "-BIN") {
            format = 0;
        }
        if(string(argv[i]) == "-HEX") {
            format = 1;
        }
    }
    if (source_file.is_open() && output_file.is_open()) {
        p4();
        source_file.close();
        output_file.close();
    }
    return 0;
}
