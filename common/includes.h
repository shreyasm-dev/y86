#ifndef INCLUDES_H
#define INCLUDES_H

#define _ source[i]
#define __ source[i + 1]
#define _$ source[i++]

#define nr ((byte[]){(_ & 0xf0) >> 4, _ & 0xf})
#define nw ((source[i + 3] << 24) | (source[i + 2] << 16) | (source[i + 1] << 8) | source[i])

#define _$$(i) ((instruction & 0xf0) == instructions.i)

#include <libc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "instructions.h"
#include "registers.h"
#include "util.h"

#endif
