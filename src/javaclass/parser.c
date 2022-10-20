#include "parser.h"
#include "helpers.h"

void parse_class_file(class_file_t* class_file, FILE* file) {
	class_file->magic = read_u4(file);
	class_file->minor_version = read_u2(file);
	class_file->major_version = read_u2(file);

	class_file->constant_pool_count = read_u2(file);
	class_file->constant_pool = (cp_info_t*)malloc((class_file->constant_pool_count - 1) * sizeof(cp_info_t));

	for(u2 i = 0; i < class_file->constant_pool_count - 1; i++) {
		cp_info_t* cp_info = &class_file->constant_pool[i];
		cp_info->tag = read_u1(file);

		switch(cp_info->tag) {
			case CONSTANT_Methodref:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_Methodref_info));

				CONSTANT_Methodref_info* methodref_info = (CONSTANT_Methodref_info*)cp_info->info;
				methodref_info->class_index = read_u2(file);
				methodref_info->name_and_type_index = read_u2(file);
				break;
			case CONSTANT_Class:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_Class_info));

				CONSTANT_Class_info* class_info = (CONSTANT_Class_info*)cp_info->info;
				class_info->name_index = read_u2(file);
				break;
			case CONSTANT_NameAndType:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_NameAndType_info));

				CONSTANT_NameAndType_info* name_and_type_info = (CONSTANT_NameAndType_info*)cp_info->info;
				name_and_type_info->name_index = read_u2(file);
				name_and_type_info->descriptor_index = read_u2(file);
				break;
			case CONSTANT_Utf8:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_Utf8_info));

				CONSTANT_Utf8_info* utf8_info = (CONSTANT_Utf8_info*)cp_info->info;
				utf8_info->length = read_u2(file);
				utf8_info->bytes = (u1*)malloc(utf8_info->length);
				fread(utf8_info->bytes, utf8_info->length, 1, file);
				break;
			case CONSTANT_Fieldref:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_Fieldref_info));

				CONSTANT_Fieldref_info* fieldref_info = (CONSTANT_Fieldref_info*)cp_info->info;
				fieldref_info->class_index = read_u2(file);
				fieldref_info->name_and_type_index = read_u2(file);
				break;
			case CONSTANT_String:
				cp_info->info = (u1*)malloc(sizeof(CONSTANT_String_info));

				CONSTANT_String_info* string_info = (CONSTANT_String_info*)cp_info->info;
				string_info->string_index = read_u2(file);
				break;
			default:
				fprintf(stderr, "Unknown cp_info->tag: %d\n", cp_info->tag);
				exit(1);
		}
	}

	class_file->access_flags = read_u2(file);
	class_file->this_class = read_u2(file);
	class_file->super_class = read_u2(file);

	class_file->interfaces_count = read_u2(file);
	class_file->interfaces = (u2*)malloc(class_file->interfaces_count * sizeof(u2));

	for(u2 i = 0; i < class_file->interfaces_count; i++) {
		// TODO: The constant_pool entry at each value of interfaces[i], where 0 ≤ i < interfaces_count, must be a CONSTANT_Class_info structure 
		class_file->interfaces[i] = read_u2(file);
	}

	class_file->fields_count = read_u2(file);
	class_file->fields = (field_info_t*)malloc(class_file->fields_count * sizeof(field_info_t));

	for(u2 i = 0; i < class_file->fields_count; i++) {
		field_info_t* field_info = &class_file->fields[i];

		field_info->access_flags = read_u2(file);
		field_info->name_index = read_u2(file);
		field_info->descriptor_index = read_u2(file);

		field_info->attributes_count = read_u2(file);
		field_info->attributes = (attribute_info_t*)malloc(field_info->attributes_count * sizeof(attribute_info_t));

		for(u2 j = 0; j < field_info->attributes_count; j++) {
			attribute_info_t* attribute_info = &field_info->attributes[j];
			attribute_info->attribute_name_index = read_u2(file);

			attribute_info->attribute_length = read_u4(file);
			attribute_info->info = (u1*)malloc(attribute_info->attribute_length);
			fread(attribute_info->info, attribute_info->attribute_length, 1, file);
		}
	}

	class_file->methods_count = read_u2(file);
	class_file->methods = (method_info_t*)malloc(class_file->methods_count * sizeof(method_info_t));

	for(u2 i = 0; i < class_file->methods_count; i++) {
		method_info_t* method_info = &class_file->methods[i];
		method_info->access_flags = read_u2(file);
		method_info->name_index = read_u2(file);
		method_info->descriptor_index = read_u2(file);
		
		method_info->attributes_count = read_u2(file);
		method_info->attributes = (attribute_info_t*)malloc(method_info->attributes_count * sizeof(attribute_info_t));

		for(u2 j = 0; j < method_info->attributes_count; j++) {
			attribute_info_t* attribute_info = &method_info->attributes[j];
			attribute_info->attribute_name_index = read_u2(file);

			attribute_info->attribute_length = read_u4(file);
			attribute_info->info = (u1*)malloc(attribute_info->attribute_length);
			fread(attribute_info->info, attribute_info->attribute_length, 1, file);
		}
	}

	class_file->attributes_count = read_u2(file);
	class_file->attributes = (attribute_info_t*)malloc(class_file->attributes_count * sizeof(class_file->attributes_count));

	for(u2 i = 0; i < class_file->attributes_count; i++) {
		attribute_info_t* attribute_info = &class_file->attributes[i];
		attribute_info->attribute_name_index = read_u2(file);

		attribute_info->attribute_length = read_u4(file);
		attribute_info->info = (u1*)malloc(attribute_info->attribute_length);
		fread(attribute_info->info, attribute_info->attribute_length, 1, file);
	}
}

void parse_code_attribute(code_attribute_t* code_attribute, u1* info, u4 attribute_length) {
	FILE* file = fmemopen(info, attribute_length, "rb");

	code_attribute->max_stack = read_u2(file);
	code_attribute->max_locals = read_u2(file);

	code_attribute->code_length = read_u4(file);
	code_attribute->code = (u1*)malloc(code_attribute->code_length);
	fread(code_attribute->code, code_attribute->code_length, 1, file);

	code_attribute->exception_table_length = read_u2(file);
	code_attribute->exception_table = (exception_t*)malloc(code_attribute->exception_table_length * sizeof(exception_t));
	for(u2 i = 0; i < code_attribute->exception_table_length; i++) {
		exception_t* exception = &code_attribute->exception_table[i];
		exception->start_pc = read_u2(file);
		exception->end_pc = read_u2(file);
		exception->handler_pc = read_u2(file);
		exception->catch_type = read_u2(file);
	}

	code_attribute->attributes_count = read_u2(file);
	code_attribute->attributes = (attribute_info_t*)malloc(code_attribute->attributes_count * sizeof(attribute_info_t));
	for(u2 i = 0; i < code_attribute->attributes_count; i++) {
		attribute_info_t* attribute_info = &code_attribute->attributes[i];
		attribute_info->attribute_name_index = read_u2(file);
		attribute_info->attribute_length = read_u4(file);

		attribute_info->info = (u1*)malloc(attribute_info->attribute_length);
		fread(attribute_info->info, attribute_info->attribute_length, 1, file);
	}

	fclose(file);
}

void free_class_file(class_file_t* class_file) {
	for(u2 i = 0; i < class_file->constant_pool_count - 1; i++) {
		switch(class_file->constant_pool[i].tag) {
			case CONSTANT_Utf8:
				CONSTANT_Utf8_info* utf8_info = (CONSTANT_Utf8_info*)class_file->constant_pool[i].info;
				free(utf8_info->bytes);
				free(class_file->constant_pool[i].info);
				break;
			default:
				free(class_file->constant_pool[i].info);
				break;
		}
	}

	free(class_file->constant_pool);
	free(class_file->interfaces);

	for(u2 i = 0; i < class_file->fields_count; i++) {
		for(u2 j = 0; j < class_file->fields[i].attributes_count; j++) {
			free(class_file->fields[i].attributes[j].info);
		}
		free(class_file->fields[i].attributes);
	}

	free(class_file->fields);

	for(u2 i = 0; i < class_file->methods_count; i++) {
		for(u2 j = 0; j < class_file->methods[i].attributes_count; j++) {
			free(class_file->methods[i].attributes[j].info);
		}
		free(class_file->methods[i].attributes);
	}

	free(class_file->methods);

	for(u2 i = 0; i < class_file->attributes_count; i++) {
		free(class_file->attributes[i].info);
	}

	free(class_file->attributes);
}

void free_code_attribute(code_attribute_t* code_attribute) {
	free(code_attribute->code);
	free(code_attribute->exception_table);

	for(u2 i = 0; i < code_attribute->attributes_count; i++) {
		free(code_attribute->attributes[i].info);
	}

	free(code_attribute->attributes);
}