#define _ source[i]
#define __ source[i + 1]

#include "includes.h"
#include "instructions.h"
#include "lexer.h"
#include "registers.h"
#include "util.h"

int address;
byte* result;

void pushb(byte value) {
  result = (byte*)realloc(result, (address + 1) * sizeof(byte));
  result[address] = value;
  address++;
}

void pushi(int value) {
  pushb((byte)((value >> 8) & 0xff));
  pushb((byte)(value & 0xff));
}

// TODO: 0x- prefix
int main(int argc, char** argv) {
  lookup_init();

  // test/argv[1]
  char* filename = (char*)malloc(strlen("test/") + strlen(argv[1]) + 1);
  strcat(filename, "test/");
  strcat(filename, argv[1]);

  char* expected_filename = (char*)malloc(strlen("test/") + strlen(argv[1]) +
                                          strlen("-expected") + 1);
  strcat(expected_filename, "test/");
  strcat(expected_filename, argv[1]);
  strcat(expected_filename, "-expected");

  // read files
  char* str = read_string(filename);
  char* expected = read_string(expected_filename);

  expected[strlen(expected) - 1] = '\0';

  // tokenise
  char** source;
  int n;
  tokenise(&source, &n, str);

  // init
  address = 0;
  result = (byte*)malloc(0);

  // assemble
  for (int i = 0; i < n;) {
    if (eq(_, "halt")) {         // halt -> halt
      pushb(instructions.halt);  // halt
      i++;
    } else if (eq(_, "nop")) {  // nop -> nop
      pushb(instructions.nop);  // nop
      i++;
    } else if (eq(_, "rrmovl")) {  // rrmovl a, b -> rrmovl a b
      pushb(instructions.rrmovl);  // rrmovl
      i++;

      pushb((r(_) << 4) | r(__));  // a, b
      i += 2;
    } else if (eq(_, "irmovl")) {  // irmovl v, b -> irmovl f/b v
      pushb(instructions.irmovl);  // irmovl
      i++;

      int v = atoi(_);
      i++;

      pushb(0xf0 | r(_));  // b
      i++;

      pushi(v);                 // v
    } else if (eq(_, "ret")) {  // ret -> ret
      pushb(instructions.ret);  // ret
      i++;
    } else {
      printf("unknown instruction: %s\n", _);
      return 1;
    }
  }

  char* result_str = (char*)malloc(0);

  for (int i = 0; i < address; i++) {
    char* temp = (char*)malloc(3);
    sprintf(temp, "%02x", result[i]);
    result_str = (char*)realloc(result_str, strlen(result_str) + 3 + 1);
    strcat(result_str, temp);

    printf("%02x ", result[i]);
  }

  printf("— ");

  if (strcmp(result_str, expected) == 0) {
    printf("success\n");
  } else {
    printf("failure - expected %s, got %s\n", expected, result_str);
  }
}
