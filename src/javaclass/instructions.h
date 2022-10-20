#pragma once

#include "types.h"

// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html

typedef struct instruction_s instruction_t;

struct instruction_s {
	char mnemonic[32];
	u1 argc;
};

static instruction_t instruction_table[256] = {
	[0x00] = { "nop", 0 },
	[0x03] = { "iconst_0", 0 },
	[0x04] = { "iconst_1", 0 },
	[0x05] = { "iconst_2", 0 },
	[0x06] = { "iconst_3", 0 },
	[0x12] = { "ldc", 1 },
	[0x1A] = { "iload_0", 0 },
	[0x1B] = { "iload_1", 0 },
	[0x2A] = { "aload_0", 0 },
	[0x3C] = { "istore_1", 0 },
	[0x60] = { "iadd", 0 },
	[0x64] = { "isub", 0 },
	[0x6F] = { "ddiv", 0 },
	[0x70] = { "irem", 0 },
	[0x84] = { "iinc", 2 },
	[0x87] = { "i2d", 0 },
	[0x9A] = { "ifne", 2 },
	[0xA2] = { "if_icmpge", 2 },
	[0xA7] = { "goto", 2 },
	[0xAF] = { "dreturn", 0 },
	[0xB1] = { "return", 0 },
	[0xB2] = { "getstatic", 2 },
	[0xB6] = { "invokevirtual", 2 },
	[0xB7] = { "invokespecial", 2 },
	[0xB8] = { "invokestatic", 2 },
};
