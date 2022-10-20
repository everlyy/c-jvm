// Don't look at this file, it's a big mess :^)

#include "printer.h"

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

size_t printer_indent = 0;
void p_set_indent(size_t indent) {
	printer_indent = indent;
}

// Wrapper around printf to make indentation easier :-)
void p_printf(char* fmt, ...) {
	for(size_t i = 0; i < printer_indent; i++)
		putchar(' ');

	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void print_buffer(u1* buffer, size_t size) {
	printer_indent += 2;

	p_printf("");
	for(size_t i = 0; i < size; i++)
		printf("%02hX ", buffer[i]);
	putchar('\n');

	printer_indent -= 2;
}

void print_binary(u2 value) {
	if(value > 1)
		print_binary(value / 2);
	printf("%d", value % 2);
}

void print_constant_info(u1 tag, u1* info) {
	printer_indent += 2;

	switch(tag) {
		case CONSTANT_Class:
			p_printf("[CONSTANT_Class]\n");
			CONSTANT_Class_info* class_info = (CONSTANT_Class_info*)info;

			p_printf("name_index: %d\n", class_info->name_index);
			break;
		case CONSTANT_Fieldref:
			p_printf("[CONSTANT_Fieldref]\n");
			CONSTANT_Fieldref_info* fieldref_info = (CONSTANT_Fieldref_info*)info;

			p_printf("class_index: %d\n", fieldref_info->class_index);
			p_printf("name_and_type_index: %d\n", fieldref_info->name_and_type_index);
			break;
		case CONSTANT_Methodref:
			p_printf("[CONSTANT_Methodref]\n");
			CONSTANT_Methodref_info* methodref_info = (CONSTANT_Methodref_info*)info;

			p_printf("class_index: %d\n", methodref_info->class_index);
			p_printf("name_and_type_index: %d\n", methodref_info->name_and_type_index);
			break;
		case CONSTANT_InterfaceMethodref:
			p_printf("[CONSTANT_InterfaceMethodref]\n");
			break;
		case CONSTANT_String:
			p_printf("[CONSTANT_String]\n");
			CONSTANT_String_info* string_info = (CONSTANT_String_info*)info;

			p_printf("string_index: %d\n", string_info->string_index);
			break;
		case CONSTANT_Integer:
			p_printf("[CONSTANT_Integer]\n");
			break;
		case CONSTANT_Float:
			p_printf("[CONSTANT_Float]\n");
			break;
		case CONSTANT_Long:
			p_printf("[CONSTANT_Long]\n");
			break;
		case CONSTANT_Double:
			p_printf("[CONSTANT_Double]\n");
			break;
		case CONSTANT_NameAndType:
			p_printf("[CONSTANT_NameAndType]\n");
			CONSTANT_NameAndType_info* name_and_type_info = (CONSTANT_NameAndType_info*)info;

			p_printf("name_index: %d\n", name_and_type_info->name_index);
			p_printf("descriptor_index: %d\n", name_and_type_info->descriptor_index);
			break;
		case CONSTANT_Utf8:
			p_printf("[CONSTANT_Utf8]\n");
			CONSTANT_Utf8_info* utf8_info = (CONSTANT_Utf8_info*)info;

			p_printf("length: %d\n", utf8_info->length);
			p_printf("bytes: \"%s\"\n", utf8_info->bytes);
			break;
		case CONSTANT_MethodHandle:
			p_printf("[CONSTANT_MethodHandle]\n");
			break;
		case CONSTANT_MethodType:
			p_printf("[CONSTANT_MethodType]\n");
			break;
		case CONSTANT_InvokeDynamic:
			p_printf("[CONSTANT_InvokeDynamic]\n");
			break;
		default:
			printf("%d: ???\n", tag);
			break;
	}

	printer_indent -= 2;
}

void print_cp_info(cp_info_t* cp_info) {
	printer_indent += 2;

	p_printf("tag: %d\n", cp_info->tag);
	p_printf("info: @ %p\n", cp_info->info);
	
	print_constant_info(cp_info->tag, cp_info->info);

	printer_indent -= 2;
}

void print_attribute_info(attribute_info_t* attribute_info) {
	printer_indent += 2;

	p_printf("attribute_name_index: %d\n", attribute_info->attribute_name_index);
	p_printf("attribute_length: %d\n", attribute_info->attribute_length);
	p_printf("info: @ %p\n", attribute_info->info);
	print_buffer(attribute_info->info, attribute_info->attribute_length);

	printer_indent -= 2;
}

void print_field_info(field_info_t* field_info) {
	printer_indent += 2;

	p_printf("access_flags: ");
	print_binary(field_info->access_flags);
	printf("\n");
	p_printf("name_index: %d\n", field_info->name_index);
	p_printf("descriptor_index: %d\n", field_info->descriptor_index);
	p_printf("attributes_count: %d\n", field_info->attributes_count);
	p_printf("attributes: @ %p\n", field_info->attributes);

	for(u2 j = 0; j < field_info->attributes_count; j++) {
		attribute_info_t* attribute_info = &field_info->attributes[j];
		print_attribute_info(attribute_info);
	}

	printer_indent -= 2;
}

void print_method_info(method_info_t* method_info) {
	printer_indent += 2;

	p_printf("access_flags: ");
	print_binary(method_info->access_flags);
	putchar('\n');
	p_printf("name_index: %d\n", method_info->name_index);
	p_printf("descriptor_index: %d\n", method_info->descriptor_index);
	p_printf("attributes_count: %d\n", method_info->attributes_count);
	p_printf("attributes: @ %p\n", method_info->attributes);

	for(u2 j = 0; j < method_info->attributes_count; j++) {
		attribute_info_t* attribute_info = &method_info->attributes[j];
		print_attribute_info(attribute_info);
	}

	printer_indent -= 2;
}

void print_class_file(class_file_t* class_file) {
	p_printf("class_file: @ %p\n", class_file);

	printer_indent += 2;

	p_printf("magic: %04X\n", class_file->magic);
	p_printf("minor_version: %d\n", class_file->minor_version);
	p_printf("major_version: %d\n", class_file->major_version);
	p_printf("constant_pool_count: %d\n", class_file->constant_pool_count);
	p_printf("constant_pool: @ %p\n", class_file->constant_pool);

	printer_indent += 2;
	for(u2 i = 0; i < class_file->constant_pool_count - 1; i++) {
		cp_info_t* cp_info = &class_file->constant_pool[i];
		p_printf("%d:\n", i);
		print_cp_info(cp_info);
	}
	printer_indent -= 2;

	p_printf("access_flags: ");
	print_binary(class_file->access_flags);
	putchar('\n');
	p_printf("this_class: %d\n", class_file->this_class);
	p_printf("super_class: %d\n", class_file->super_class);
	p_printf("interfaces_count: %d\n", class_file->interfaces_count);
	p_printf("interfaces: @ %p\n", class_file->interfaces);

	printer_indent += 2;
	for(u2 i = 0; i < class_file->interfaces_count; i++) {
		p_printf("%d: %d\n", i, class_file->interfaces[i]);
	}
	printer_indent -= 2;

	p_printf("fields_count: %d\n", class_file->fields_count);
	p_printf("fields: @ %p\n", class_file->fields);

	printer_indent += 2;
	for(u2 i = 0; i < class_file->fields_count; i++) {
		field_info_t* field_info = &class_file->fields[i];
		print_field_info(field_info);
	}
	printer_indent -= 2;

	p_printf("methods_count: %d\n", class_file->methods_count);
	p_printf("methods: @ %p\n", class_file->methods);

	printer_indent += 2;
	for(u2 i = 0; i < class_file->methods_count; i++) {
		method_info_t* method_info = &class_file->methods[i];
		p_printf("%d: \n", i);
		print_method_info(method_info);
	}
	printer_indent -= 2;

	printer_indent -= 2;
}

void print_exception(exception_t* exception) {
	printer_indent += 2;

	p_printf("start_pc: %d\n", exception->start_pc);
	p_printf("end_pc: %d\n", exception->end_pc);
	p_printf("handler_pc: %d\n", exception->handler_pc);
	p_printf("catch_type: %d\n", exception->catch_type);

	printer_indent -= 2;
}

void print_code_attribute(code_attribute_t* code_attribute) {
	p_printf("code_attribute: \n");

	printer_indent += 2;

	p_printf("max_stack: %d\n", code_attribute->max_stack);
	p_printf("max_locals: %d\n", code_attribute->max_locals);
	p_printf("code_length: %d\n", code_attribute->code_length);
	p_printf("code: @ %p\n", code_attribute->code);
	print_buffer(code_attribute->code, code_attribute->code_length);
	p_printf("exception_table_length: %d\n", code_attribute->exception_table_length);
	p_printf("exception_table: @ %p\n", code_attribute->exception_table);

	printer_indent += 2;
	for(u2 i = 0; i < code_attribute->exception_table_length; i++) {
		exception_t* exception = &code_attribute->exception_table[i];
		p_printf("%d: \n", i);
		print_exception(exception);
	}
	printer_indent -= 2;

	p_printf("attributes_count: %d\n", code_attribute->attributes_count);
	p_printf("attributes: @ %p\n", code_attribute->attributes);

	for(u2 j = 0; j < code_attribute->attributes_count; j++) {
		attribute_info_t* attribute_info = &code_attribute->attributes[j];
		print_attribute_info(attribute_info);
	}

	printer_indent -= 2;
}