#include "../common/includes.h"
#include "store.h"

int main(int argc, char** argv) {
  init_store();

  char* filename =
      (char*)malloc(strlen(argv[1]) + strlen("/test/") + strlen(argv[2]) + 1);
  strcat(filename, argv[1]);
  strcat(filename, "/test/");
  strcat(filename, argv[2]);

  long len = 0;
  byte* buf = read_ascii_hex(filename, &len);
}
