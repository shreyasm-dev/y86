#include "../common/includes.h"
#include "lexer.h"

int i = 0;
int address = 0;
byte* result;
char** source;
deferred_address* deferred_addresses;
int deferred_n = 0;

void pushb(byte value) { push(result, value, address); }

void pushi(int value) {
  pushb((byte)(value & 0xff));
  pushb((byte)((value >> 8) & 0xff));
  pushb((byte)((value >> 16) & 0xff));
  pushb((byte)((value >> 24) & 0xff));
}

void expect(char* expected) {
  if (_ == NULL || !eq(_, expected)) {
    error("expected \"%s\", got \"%s\"", expected, _);
  }

  i++;
}

int expect_number(char* s, bool push) {
  int value = 0;

  if (s[0] == '$') {
    value = strtol(s + 1, NULL, 16);
  } else if (s[0] == '0' && s[1] == 'x') {
    value = strtol(s + 2, NULL, 16);
  } else if (s[0] >= '0' && s[0] <= '9') {
    value = atoi(s);
  } else if (s[0] != '(') {
    struct deferred_address deferred = {
        .address = address,
        .label = s,
    };

    push(deferred_addresses, deferred, deferred_n);

    value = 0;
  }

  if (push) {
    pushi(value);
  }

  return value;
}

void expect_spaces() {
  // it's one character if it's whitespace anyway, so only looking at the first
  // character is fine
  if (_ != NULL && ceq_any(_[0], " \t")) {
    i++;
    expect_spaces();
  }
}

void expect_whitespace() {
  if (_ != NULL && ceq_any(_[0], " \t\n\r")) {
    i++;
    expect_whitespace();
  }
}

int main(int argc, char** argv) {
  struct array_map address_lookup = create_map();

  // argv[1]/test/argv[2]
  char* filename = (char*)malloc(strlen(argv[1]) + strlen("/test/") +
                                  strlen(argv[2]) + 1);
  strcat(filename, argv[1]);
  strcat(filename, "/test/");
  strcat(filename, argv[2]);

  char* expected_filename = (char*)malloc(strlen(argv[1]) +
                                          strlen("/test/") +
                                          strlen(argv[2]) + 1);
  strcat(expected_filename, argv[1]);
  strcat(expected_filename, "/test/");
  strcat(expected_filename, argv[2]);
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
  deferred_addresses = (deferred_address*)malloc(0);

  // assemble
  while (i < n) {
    expect_whitespace();

    if (eq(_, ".pos")) {  // .pos x
      i++;

      expect_spaces();

      int x = expect_number(_, false);  // x
      i++;

      if (x < address) {
        error("cannot move address back (current address is %d, got %d)",
              address, x);
      }

      while (address < x) {
        pushb(instructions.nop);
      }
    } else if (eq(_, ".align")) {  // .align x
      i++;

      expect_spaces();

      int x = expect_number(_, false);
      i++;

      if (x <= 0) {
        error("alignment must be positive (got %d)", x);
      }

      while (address % x != 0) {
        pushb(instructions.nop);
      }
    } else if (eq_any(_, (char*[]){".long", ".quad"},
                      2)) {  // .long x // .quad x
      i++;

      expect_spaces();

      expect_number(_, true);  // x
      i++;
    } else if (_[strlen(_) - 1] == ':') {  // label
      char* label = (char*)malloc(strlen(_));
      strncpy(label, _, strlen(_) - 1);
      i++;

      if (get(address_lookup, label) != -1) {
        error("duplicate label \"%s\"", label);
      }

      set(&address_lookup, label, address);
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

      expect_spaces();

      int a = r(_);
      i++;

      expect(",");
      expect_spaces();

      pushb((a << 4) | r(_));  // a, b
      i++;
    } else if (eq(_, "irmovl")) {  // irmovl v, b -> irmovl f/b v
      pushb(instructions.irmovl);  // irmovl
      i++;

      expect_spaces();

      char* v = _;
      i++;

      expect(",");
      expect_spaces();

      pushb(0xf0 | r(_));  // b
      i++;

      expect_number(v, true);      // v
    } else if (eq(_, "rmmovl")) {  // rmmovl a, d(b) -> rmmovl a b d
      pushb(instructions.rmmovl);  // rmmovl
      i++;

      expect_spaces();

      int a = r(_);
      i++;

      expect(",");
      expect_spaces();

      char* d = _;
      i++;

      expect("(");

      pushb((a << 4) | r(_));  // a, b
      i++;

      expect(")");

      expect_number(d, true);      // d
    } else if (eq(_, "mrmovl")) {  // mrmovl d(b), a -> mrmovl a b d
      pushb(instructions.mrmovl);  // mrmovl
      i++;

      expect_spaces();

      char* d = _;
      i++;

      expect("(");

      int b = r(_);
      i++;

      expect(")");
      expect(",");
      expect_spaces();

      pushb((r(_) << 4) | b);  // a, b
      i++;

      expect_number(d, true);  // d
    } else if (eq_any(_,
                      (char*[]){"call", "jmp", "jle", "jl", "je", "jne", "jge",
                                "jg"},
                      8)) {  // [call/jmp/jle/jl/je/jne/jge/jg] l
      pushb(s(_));           // [call/jmp/jle/jl/je/jne/jge/jg]
      i++;

      expect_spaces();

      expect_number(_, true);  // l
      i++;
    } else if (eq_any(_, (char*[]){"pushl", "popl"},
                      2)) {  // [pushl/popl] a -> [pushl/popl] a/f
      pushb(s(_));           // [pushl/popl]
      i++;

      expect_spaces();

      pushb((r(_) << 4) | 0xf);  // a/f
      i++;
    } else {
      error("unknown instruction \"%s\"", _);
    }

    expect_whitespace();
  }

  // resolve all addresses
  for (int i = 0; i < deferred_n; i++) {
    int a = get(address_lookup, deferred_addresses[i].label);

    if (a == -1) {
      error("unknown label \"%s\"", deferred_addresses[i].label);
    }

    int address = deferred_addresses[i].address;

    result[address] = (byte)(a & 0xff);
    result[address + 1] = (byte)((a >> 8) & 0xff);
    result[address + 2] = (byte)((a >> 16) & 0xff);
    result[address + 3] = (byte)((a >> 24) & 0xff);
  }

  char* result_str = (char*)malloc(0);

  for (int i = 0; i < address; i++) {
    char* temp = (char*)malloc(3);
    snprintf(temp, sizeof(temp), "%02x", result[i]);

    result_str =
        (char*)realloc(result_str, strlen(result_str) + strlen(temp) + 1);
    strcat(result_str, temp);

    printf("%02x ", result[i]);
  }

  printf("— ");

  if (eq(result_str, expected)) {
    success("success");
  }

  error("failure - expected %s, got %s", expected, result_str);
}
