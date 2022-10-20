#pragma once

#include "types.h"
#include <stddef.h>

void p_set_indent(size_t indent);

void print_buffer(u1*, size_t);
void print_constant_info(u1 tag, u1* info);
void print_cp_info(cp_info_t*);
void print_attribute_info(attribute_info_t*);
void print_field_info(field_info_t*);
void print_method_info(method_info_t*);
void print_class_file(class_file_t*);
void print_exception(exception_t*);
void print_code_attribute(code_attribute_t*);