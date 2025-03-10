# y86-assembler

OCS35 project

## Usage

Use `./run <test>` to test against expected output. For clarity, all tests are hand-written (i.e. not copied from the output the assembler itself produces) and so should be correct and useful to test against.

Available tests:

- `all` - tests all instruction patterns (i.e. tests `call` but not `jmp`, since they take the same arguments and testing both would be redundant because of how it's implemented)
- `practice` - from the practice problem set
