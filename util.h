struct instructions {
  int halt;
  int nop;

  int rrmovl;
  int cmovle;
  int cmovl;
  int cmove;
  int cmovne;
  int cmovge;
  int cmovg;

  int irmovl;
  int rmmovl;
  int mrmovl;

  int addl;
  int subl;
  int andl;
  int xorl;

  int jmp;
  int jle;
  int jl;
  int je;
  int jne;
  int jge;
  int jg;
};

struct registers {
  int eax;
  int ecx;
  int edx;
  int ebx;
  int esp;
  int ebp;
  int esi;
  int edi;
};

extern const struct instructions instructions;
extern const struct registers registers;
