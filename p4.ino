#include <vector>

const uint8_t operand[] = {13, 12, 11, 10}; // 13 highest bit, output
const uint8_t ins[] = {9, 8}; // 9 highest bit, output
//const uint8_t out_reg[] = {7, 6, 5, 4}; // 7 highest bit, input
const uint8_t pc_addr[] = {7, 6, 5, 4}; // 7 highest bit, input
const uint8_t clk = 3;

#define clk_speed 40

using namespace std;

/*
0 NOP
1 LDA
2 ADD
3 JMP
*/

uint8_t pc;

/*vector<String> program = {
  "lda 0",
  "add 1",
  "add 2",
  "add 4",
  "add 8",
  "lda 1",
  "add 15",
  "jmp 0",
};*/

vector<String> program = {
  "lda 1",
  "add 0",
  "add 1",
  "add 3",
  "add 7",
  "add 15",
  "add 7",
  "add 3",
  "add 1",
  "add 0",  
  "jmp 1",
};

void setup() {
  Serial.begin(115200); // Init
  // put your setup code here, to run once:
  for (int i = 0; i < 4; i++) {
    pinMode(operand[i], OUTPUT);
    pinMode(pc_addr[i], INPUT);
  }
  pinMode(ins[0], OUTPUT);
  pinMode(ins[1], OUTPUT);
  pinMode(clk, OUTPUT);
  run_with_pc();
}

void clk_pulse() {
  digitalWrite(clk, HIGH);
  delay(clk_speed);
  digitalWrite(clk, LOW);
  delay(clk_speed);
}

void cpu_write(uint8_t dataI, uint8_t insI) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(operand[i], (dataI >> (3 - i)) & 1);
  }
  for (int i = 0; i < 2; i++) {
    digitalWrite(ins[i], (insI >> (1 - i)) & 1);
  }
}

/*uint8_t cpu_read() {
  int or_reg = 0;
  for (int i = 0; i < 4; i++) {
    int value = digitalRead(out_reg[i]);
    or_reg |= (value << (3 - i));
  }
  return or_reg;
}*/

void test_pc() {
  while(true) {
    uint8_t pc_val = read_pc();
    Serial.print("PC: ");
    for (int i = 3; i >= 0; i--) {
      Serial.print((pc_val >> i) & 1);
    }
    Serial.println();
    clk_pulse();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

uint8_t read_pc() {
  uint8_t pc_temp = 0;
  for (int i = 0; i < 4; i++) {
    int value = digitalRead(pc_addr[i]);
    pc_temp |= (value << (3 - i));
  }
  return pc_temp;
}


void run_with_pc(){
  while(true) {
    clk_pulse();
    pc = read_pc();
    if(program[pc].startsWith("nop")) {
      cpu_write(0,0);
    }
    else if(program[pc].startsWith("lda ")) {
      int imm = program[pc].substring(4).toInt();
      cpu_write(imm, 1);
    }
    else if(program[pc].startsWith("add ")) {
      int imm = program[pc].substring(4).toInt();
      cpu_write(imm, 2);
    }
    else if(program[pc].startsWith("jmp ")) {
      int imm = program[pc].substring(4).toInt();
      cpu_write(imm, 3);
    }
  }
}
void execute() {
  for(pc = 0; pc < program.size(); pc++) {
    //cout << program[i] << endl;
    if(program[pc].startsWith("nop")) {
      cpu_write(0,0);
    }
    if(program[pc].startsWith("lda ")) {
      int imm = program[pc].substring(4).toInt();
      cpu_write(imm, 1);
    }
    if(program[pc].startsWith("add ")) {
      int imm = program[pc].substring(4).toInt();
      cpu_write(imm, 2);
    }
    if(program[pc].startsWith("jmp ")) {
      int imm = program[pc].substring(4).toInt();
      clk_pulse();
      pc = imm;
    }
    clk_pulse();
  }
}
