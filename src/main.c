#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "javaclass/parser.h"
#include "javaclass/printer.h"
#include "javaclass/helpers.h"
#include "javaclass/instructions.h"

void add_byte_to_buffer(u1 byte, char* buffer) {
	char temp[4];
	sprintf(temp, "%02X ", byte);
	strcat(buffer, temp);
}

void disassemble_code(u1* code, u4 code_length) {
	FILE* file = fmemopen(code, code_length, "rb");
	
	while(!feof(file)) {
		printf("    ");

		char buffer[32];
		buffer[0] = 0x00;

		u1 opcode = read_u1(file);
		add_byte_to_buffer(opcode, buffer);

		instruction_t instruction = instruction_table[opcode];

		for(u1 i = 0; i < instruction.argc; i++) {
			u1 arg = read_u1(file);
			add_byte_to_buffer(arg, buffer);
		}

		printf(buffer);

		size_t buffer_size = strlen(buffer);
		for(size_t i = 0; i < 12 - buffer_size; i++) {
			printf(" ");
		}

		printf("%s\n", instruction.mnemonic);
	}

	printf("\n");
	fclose(file);
}

void disassemble_methods(class_file_t* class_file) {
	for(u2 i = 0; i < class_file->methods_count; i++) {
		method_info_t* method_info = &class_file->methods[i];

		cp_info_t cp_info = class_file->constant_pool[method_info->name_index - 1];
		if(cp_info.tag != CONSTANT_Utf8) {
			printf("Method %d has invalid name index %d (tag: %d)\n", i, method_info->name_index, cp_info.tag);
			return;
		}

		CONSTANT_Utf8_info method_name = *((CONSTANT_Utf8_info*)cp_info.info);
		printf("%.*s: \n", method_name.length, method_name.bytes);

		for(u2 j = 0; j < method_info->attributes_count; j++) {
			attribute_info_t* attribute_info = &method_info->attributes[j];

			code_attribute_t code_attribute;

			parse_code_attribute(&code_attribute, attribute_info->info, attribute_info->attribute_length);

			disassemble_code(code_attribute.code, code_attribute.code_length);

			free_code_attribute(&code_attribute);
		}
	}
}

int main(void) {
	const char* class_file_path = "Main.class";
	FILE* file = fopen(class_file_path, "rb");

	if(!file) {
		fprintf(stderr, "Error opening %s: %s\n", class_file_path, strerror(errno));
		return 1;
	}

	class_file_t class_file;
	memset(&class_file, 0, sizeof(class_file));

	parse_class_file(&class_file, file);

	print_class_file(&class_file);

	disassemble_methods(&class_file);

	free_class_file(&class_file);

	fclose(file);
}