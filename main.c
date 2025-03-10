#include "includes.h"
#include "instructions.h"
#include "lexer.h"
#include "registers.h"
#include "util.h"

int i = 0;
int address = 0;
byte* result;
char** source;

void pushb(byte value) { push(result, value, address); }

void pushi(int value) {
  pushb((byte)((value >> 8) & 0xff));
  pushb((byte)(value & 0xff));
}

void expect(char* expected) {
  if (!eq(_, expected)) {
    error("expected \"%s\", got \"%s\"", expected, _);
  }

  i++;
}

void expect_space() {
  // it's one character if it's whitespace anyway, so only looking at the first
  // character is fine
  if (!ceq_any(_[0], " \t")) {
    error("expected space, got \"%s\"", _);
  }

  i++;
}

void expect_newline() {
  if (!ceq_any(_[0], "\n\r")) {
    error("expected newline, got \"%s\"", _);
  }

  i++;
}

int main(int argc, char** argv) {
  struct array_map address_lookup = create_map();

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
  expected[strlen(expected) - 1] = '\0';  // remove final newline

  // remove spaces and newlines
  char** expected_temp;
  int n_expected = 0;
  tokenise(&expected_temp, &n_expected, expected, false);

  expected = (char*)malloc(0);
  for (int i = 0; i < n_expected; i++) {
    expected = (char*)realloc(expected,
                              strlen(expected) + strlen(expected_temp[i]) + 1);
    strcat(expected, expected_temp[i]);
  }

  // tokenise
  int n;
  tokenise(&source, &n, str, true);

  // init
  result = (byte*)malloc(0);

  // assemble
  while (i < n) {
    bool newline = true;

    if (eq(_, ".pos")) {  // .pos x
      i++;

      expect_space();

      int new = atoi(_);  // x
      i++;

      if (new < address) {
        error("cannot move address back (current address is %d)", address);
      }

      while (address < new) {
        pushb(instructions.nop);
      }
    } else if (eq(_, ".align")) {  // .align x
      i++;

      expect_space();

      int new = atoi(_);  // x
      i++;

      if (new <= 0) {
        error("alignment must be positive (got %d)", new);
      }

      while (address % new != 0) {
        pushb(instructions.nop);
      }
    } else if (eq_any(_, (char*[]){".long", ".quad"},
                      2)) {  // .long x // .quad x
      i++;

      expect_space();

      char* s = _;
      i++;

      if (s[0] != '0' || s[1] != 'x') {
        error("expected hexadecimal number beginning with 0x, got \"%s\"", s);
      }

      pushi(strtol(s + 2, NULL, 16));      // x
    } else if (_[strlen(_) - 1] == ':') {  // label
      char* label = (char*)malloc(strlen(_));
      strncpy(label, _, strlen(_) - 1);
      i++;

      expect_space();

      if (get(address_lookup, label) != -1) {
        error("duplicate label \"%s\"", label);
      }

      set(&address_lookup, label, address);
      newline = false;
    } else if (eq_any(_, (char*[]){"halt", "nop", "ret"},
                      3)) {  // [halt/nop/ret] -> [halt/nop/ret]
      pushb(s(_));           // [halt/nop/ret]
      i++;
    } else if (
        eq_any(
            _,
            (char*[]){"rrmovl", "addl", "subl", "andl", "xorl", "cmovle",
                      "cmovl", "cmove", "cmovne", "cmovge", "cmovg"},
            11)) {  // [rrmovl/addl/subl/andl/xorl/cmovle/cmovl/cmove/cmovne/cmovge/cmovg]
                    // a, b ->
                    // [rrmovl/addl/subl/andl/xorl/cmovle/cmovl/cmove/cmovne/cmovge/cmovg]
                    // a b
      pushb(s(
          _));  // [rrmovl/addl/subl/andl/xorl/cmovle/cmovl/cmove/cmovne/cmovge/cmovg]
      i++;

      expect_space();

      int a = r(_);
      i++;

      expect(",");
      expect_space();

      pushb((a << 4) | r(_));  // a, b
      i++;
    } else if (eq(_, "irmovl")) {  // irmovl v, b -> irmovl f/b v
      pushb(instructions.irmovl);  // irmovl
      i++;

      expect_space();

      char* s = _;
      i++;

      if (s[0] != '$') {
        error(
            "expected hexadecimal immediate value beginning with $, got \"%s\"",
            s);
      }

      expect(",");
      expect_space();

      pushb(0xf0 | r(_));  // b
      i++;

      pushi(strtol(s + 1, NULL, 16));  // v
    } else if (eq(_, "rmmovl")) {      // rmmovl a, d(b) -> rmmovl a b d
      pushb(instructions.rmmovl);      // rmmovl
      i++;

      expect_space();

      int a = r(_);
      i++;

      expect(",");
      expect_space();

      char* s = _;
      i++;

      if (s[0] != '$') {
        error(
            "expected hexadecimal immediate value beginning with $, got \"%s\"",
            s);
      }

      expect("(");

      pushb((a << 4) | r(_));  // a, b
      i++;

      expect(")");

      pushi(strtol(s + 1, NULL, 16));  // d
    } else if (eq(_, "mrmovl")) {      // mrmovl d(b), a -> mrmovl a b d
      pushb(instructions.mrmovl);      // mrmovl
      i++;

      expect_space();

      char* s = _;
      i++;

      if (s[0] != '$') {
        error(
            "expected hexadecimal immediate value beginning with $, got \"%s\"",
            s);
      }

      expect("(");

      int b = r(_);
      i++;

      expect(")");
      expect(",");
      expect_space();

      pushb((r(_) << 4) | b);  // a, b
      i++;

      pushi(strtol(s + 1, NULL, 16));  // d
    } else if (eq_any(_,
                      (char*[]){"call", "jmp", "jle", "jl", "je", "jne", "jge",
                                "jg"},
                      8)) {  // [call/jmp/jle/jl/je/jne/jge/jg] l
      pushb(s(_));           // [call/jmp/jle/jl/je/jne/jge/jg]
      i++;

      expect_space();

      int l = get(address_lookup, _);

      if (l == -1) {
        error("unknown label \"%s\"", _);
      }

      pushi(l);  // l
      i++;
    } else if (eq_any(_, (char*[]){"pushl", "popl"},
                      2)) {  // [pushl/popl] a -> [pushl/popl] a/f
      pushb(s(_));           // [pushl/popl]
      i++;

      expect_space();

      pushb((r(_) << 4) | 0xf);  // a/f
      i++;
    } else {
      error("unknown instruction \"%s\"", _);
    }

    if (newline) {
      expect_newline();
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
    success("success");
  }

  error("failure - expected %s, got %s", expected, result_str);
}
