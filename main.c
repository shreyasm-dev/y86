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

  // remove spaces and newlines
  char** expected_temp;
  int n_expected = 0;
  tokenise(&expected_temp, &n_expected, expected);

  expected = (char*)malloc(0);
  for (int i = 0; i < n_expected; i++) {
    expected = (char*)realloc(expected,
                              strlen(expected) + strlen(expected_temp[i]) + 1);
    strcat(expected, expected_temp[i]);
  }

  // tokenise
  char** source;
  int n;
  tokenise(&source, &n, str);

  // init
  address = 0;
  result = (byte*)malloc(0);

  // assemble
  for (int i = 0; i < n;) {
    if (eq(_, ".pos")) {  // .pos x
      i++;

      int new = atoi(_);  // x
      i++;

      if (new < address) {
        printf("cannot move address back\n");
        return 1;
      }

      while (address < new) {
        pushb(instructions.nop);
      }
    } else if (eq(_, ".align")) {  // .align x
      i++;

      int new = atoi(_);  // x
      i++;

      if (new <= 0) {
        printf("alignment must be positive\n");
        return 1;
      }

      while (address % new != 0) {
        pushb(instructions.nop);
      }
    } else if (eq(_, ".long") || eq(_, ".quad")) {
      i++;

      int value = atoi(_);
      i++;

      pushi(value);
    } else if (_[strlen(_) - 1] == ':') {  // label
      char* label = (char*)malloc(strlen(_));
      strncpy(label, _, strlen(_) - 1);
      i++;

      if (lookup_get(label) != -1) {
        printf("duplicate label: %s\n", label);
        return 1;
      }

      lookup_set(label, address);
    } else if (eq(_, "halt")) {  // halt -> halt
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

      pushi(v);                    // v
    } else if (eq(_, "rmmovl")) {  // rmmovl a, d(b) -> rmmovl a b d
      pushb(instructions.rmmovl);  // rmmovl
      i++;

      int a = r(_);
      i++;

      int d = atoi(_);
      i++;

      pushb((a << 4) | r(_));  // a, b
      i++;

      pushi(d);                    // d
    } else if (eq(_, "mrmovl")) {  // mrmovl d(b), a -> mrmovl a b d
      pushb(instructions.mrmovl);  // mrmovl
      i++;

      int d = atoi(_);
      i++;

      pushb((r(__) << 4) | r(_));  // a, b
      i += 2;

      pushi(d);                  // d
    } else if (eq(_, "call")) {  // call l -> call l
      pushb(instructions.call);  // call
      i++;

      int l = lookup_get(_);

      if (l == -1) {
        printf("unknown label: %s\n", _);
        return 1;
      }

      pushi(l);  // l
      i++;
    } else if (eq(_, "ret")) {  // ret -> ret
      pushb(instructions.ret);  // ret
      i++;
    } else if (eq(_, "pushl")) {  // pushl a -> pushl a/f
      pushb(instructions.pushl);  // pushl
      i++;

      pushb((r(_) << 4) | 0xf);  // a/f
      i++;
    } else if (eq(_, "popl")) {  // popl a -> popl a/f
      pushb(instructions.popl);  // popl
      i++;

      pushb((r(_) << 4) | 0xf);  // a/f
      i++;
    } else {
      printf("%d\n", i);
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
