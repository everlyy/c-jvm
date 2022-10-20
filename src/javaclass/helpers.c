#include "helpers.h"

u4 read_u4(FILE* file) {
	char buffer[4];
	fread(buffer, 1, 4, file);
	u4 value = ((u1)buffer[3]) | ((u1)buffer[2] << 8) | ((u1)buffer[1] << 16) | ((u1)buffer[0] << 24);
	return value;
}

u2 read_u2(FILE* file) {
	char buffer[2];
	fread(buffer, 1, 2, file);
	u2 value = ((u1)buffer[1]) | ((u1)buffer[0] << 8);
	return value;
}

u1 read_u1(FILE* file) {
	char buffer[1];
	fread(buffer, 1, 1, file);
	return (u1)*buffer;
}