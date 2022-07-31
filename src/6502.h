#ifndef MOS6502_H
#define MOS6502_H

#include "core.h"
#include "emu65.h"
#include "token.h"

#include <vector>
#include <iostream>

void compile_6502(std::vector<Token>&);

#endif