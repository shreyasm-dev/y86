# y86-assembler

OCS35 project (<https://github.com/shreyasm-dev/y86-assembler/>)

## Usage

Use `./run <test>` to test against expected output. For clarity, all tests are hand-written (i.e. not copied from the output the assembler itself produces) and so should be correct and useful to test against.

Available tests:

- `all` - tests all instruction patterns (i.e. tests `call` but not `jmp`, since they take the same arguments and testing both would be redundant because of how it's implemented)
- `practice` - from the practice problem set
- `fib` - simple fibonacci program

## Notes

- No comments

## Organisation

- `run` - script to run tests
- `tests/` - test files
  - `<test>` - test assembly
  - `<test>-expected` - expected output
- `main.c` - entrypoint
- `includes.h` - includes and some definitions
- `instructions.c`, `instructions.h` - instruction definitions and lookup
- `lexer.c`, `lexer.h` - simple tokeniser
- `registers.c`, `registers.h` - register definitions and lookup
- `utils.c`, `utils.h` - utilities
