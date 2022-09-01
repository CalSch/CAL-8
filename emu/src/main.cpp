#include <stdio.h>
#include <vector>
#include <cinttypes>
#include <chrono>
#include <thread>
#include <string>
#include <math.h>
#include "load.hpp"
#include "opcodes.hpp"
#include "util.hpp"
typedef std::uint8_t BYTE;
typedef std::uint8_t ADDR;

// Definitions
const int ram_size=256;
const int stack_size=256;


// Registers
BYTE A=0;
ADDR PC=0;
int C=0;

// Stacks
std::vector<ADDR> CS;


BYTE ram[ram_size];

bool running=true;
bool jumped=true;


void jump_to_subroutine(ADDR addr) {
	if (CS.size()>=stack_size) {
		printf("Stack overflow\n");
		running=false;
	}
	CS.push_back(PC);
	PC=addr;
	jumped=true;
}

void print_ram() {
	int width=get_window_size().ws_col;
	// int height=get_window_size().ws_row;

	//int cols=floor(width/3)-2;
	int cols=16;
	int rows=floor(ram_size/cols);

  auto print_line = [&] () {
	  printf("+----+%s+\n",repeat("-",cols*3+1).c_str());
  };

	printf("\x1b[2J\x1b[H"); // Clear

  printf("W: %d  C: %d  R: %d\n",width,cols,rows);
  
  // Offsets

  print_line();

  printf("|    | ");
  for (int i=0;i<cols;i++) {
    printf("%02x ",i);
  }
  printf("|\n");


  // RAM
  print_line();

	for (int i=0;i<rows;i++) {
		printf("| %02x | ",rows*i);
    for (int j = 0; j < cols; j++) {
      if (i*cols+j == PC) printf("\x1b[7m");
      else if (j%2!=0) printf("\x1b[38;5;250m");
      printf("%02x\x1b[0m ",ram[i*cols + j]);
    }
    printf("|\n");
	}
  
  print_line();
}

int main() {
	printf("Started.\n");
	print_ram();
	for (int i=0;i<ram_size;i++) {
		ram[i]=0x00;
	}

	printf("RAM initialized.\n");

	std::vector<BYTE> program=load(0,256,"/home/calvin/Coding/C++/CAL-8/test.bin");
	for (long unsigned int i=0;i<program.size();i++) {
		ram[i]=program.at(i);
	}

	printf("Program loaded.\n");


	while (running) {
		BYTE code=ram[PC];
    BYTE arg;
		jumped=false;
    if (code!=COMP) {
      if (ram[PC+2]==0xF) {
        arg=ram[PC+1]; // Literal value
      } else {
        arg=ram[ram[PC+1]]; // Address
      }
    }

		//printf("\x1b[A%d | %d %d %d | %d        \n",ram[0xFF],code,ram[PC+1],ram[PC+2],PC);
		// printf(            "%d | %d %d %d | %d        \n",ram[0xFF],code,ram[ram[PC+1]],ram[ram[PC+2]],PC);
		
    print_ram();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		switch (code) {
			case LDA:
				A=arg;

				break;
			case STA:
				ram[arg]=A;

				break;
			case ADD:
				A+=arg;

				break;
			case SUB:
				A-=arg;

				break;
			case JUMP:
				PC=arg;
				jumped=true;

				break;
			case JSUB:
				jump_to_subroutine(arg);

				break;
			case RET:
				PC=CS.at(CS.size()-1);
				CS.pop_back();

				break;
			case COMP:
				C=ram[ram[PC+1]]-ram[ram[PC+2]];

				break;
			case BIE:
				if (C==0) jump_to_subroutine(arg);
				break;
			case BNE:
				if (C!=0) jump_to_subroutine(arg);
				break;
			case BIG:
				if (C>0) jump_to_subroutine(arg);
				break;
			case BIL:
				if (C<0) jump_to_subroutine(arg);
				break;

			default:
				break;
		}

		if (!jumped) PC+=3;
	}

	return 0;
}
