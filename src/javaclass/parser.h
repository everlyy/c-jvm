#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void parse_class_file(class_file_t*, FILE*);
void parse_code_attribute(code_attribute_t*, u1* info, u4 attribute_length);
void free_class_file(class_file_t*);
void free_code_attribute(code_attribute_t*);