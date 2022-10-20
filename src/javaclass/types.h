#pragma once

#include <stdint.h>

typedef uint32_t u4;
typedef uint16_t u2;
typedef uint8_t u1;

typedef struct class_file_s class_file_t;
typedef struct cp_info_s cp_info_t;
typedef struct field_info_s field_info_t;
typedef struct method_info_s method_info_t;
typedef struct attribute_info_s attribute_info_t;
typedef struct exception_s exception_t;
typedef struct code_attribute_s code_attribute_t;
typedef struct flag_s flag_t;

struct class_file_s {
	u4 magic;
	u2 minor_version;
	u2 major_version;
	u2 constant_pool_count;
	cp_info_t* constant_pool;
	u2 access_flags;
	u2 this_class;
	u2 super_class;
	u2 interfaces_count;
	u2* interfaces;
	u2 fields_count;
	field_info_t* fields;
	u2 methods_count;
	method_info_t* methods;
	u2 attributes_count;
	attribute_info_t* attributes;
};

#define CONSTANT_Class				7
#define CONSTANT_Fieldref			9
#define CONSTANT_Methodref			10
#define CONSTANT_InterfaceMethodref	11
#define CONSTANT_String				8
#define CONSTANT_Integer			3
#define CONSTANT_Float				4
#define CONSTANT_Long				5
#define CONSTANT_Double				6
#define CONSTANT_NameAndType		12
#define CONSTANT_Utf8				1
#define CONSTANT_MethodHandle		15
#define CONSTANT_MethodType			16
#define CONSTANT_InvokeDynamic		18

typedef struct CONSTANT_Methodref_info {
	u2 class_index;
	u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct CONSTANT_Class_info {
	u2 name_index;
} CONSTANT_Class_info;

typedef struct CONSTANT_NameAndType_info {
	u2 name_index;
	u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct CONSTANT_Utf8_info {
	u2 length;
	u1* bytes;
} CONSTANT_Utf8_info;

typedef struct CONSTANT_Fieldref_info {
	u2 class_index;
	u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef struct CONSTANT_String_info {
	u2 string_index;
} CONSTANT_String_info;

struct cp_info_s {
	u1 tag;
	u1* info;
};

struct field_info_s {
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info_t* attributes;
};

struct method_info_s {
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	attribute_info_t* attributes;
};

struct attribute_info_s {
	u2 attribute_name_index;
	u4 attribute_length;
	u1* info;
};

struct exception_s {
	u2 start_pc;
	u2 end_pc;
	u2 handler_pc;
	u2 catch_type;
};

struct code_attribute_s {
	u2 max_stack;
	u2 max_locals;
	u4 code_length;
	u1* code;
	u2 exception_table_length;
	exception_t* exception_table;
	u2 attributes_count;
	attribute_info_t* attributes;
};