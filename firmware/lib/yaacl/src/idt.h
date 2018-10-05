#pragma once

#include <inttypes.h>
#include <stdbool.h>

struct s_yaccl_idt_std_t {
	bool rb0:1;
	bool reserved0:1;
	bool rtr:1;
	uint32_t reserved1:18;
	uint16_t ID:11;
} __attribute__((packed)) ;
typedef struct s_yaccl_idt_std_t yaccl_idt_std_t;

struct s_yaccl_idt_ext_t {
	uint8_t rb:2;
	bool rtr:1;
	uint32_t ID:29;
} __attribute__((packed));
typedef struct s_yaccl_idt_ext_t yaccl_idt_ext_t;

typedef union {
	uint32_t as_int_32;
	struct s_yaccl_idt_std_t std;
	struct s_yaccl_idt_ext_t ext;
	uint8_t data[4];
} yaccl_idt_t;


#define yaccl_make_std_id(idt,id,rtrbit) do{	\
		(idt).std.rb0 = 0; \
		(idt).std.rtr = ( (rtr) != 0); \
		(idt).reserved0 = 0; \
		(idt).reserved1 = 0; \
		(idt).std.ID = (id); \
	}while(0)

#define yaccl_make_ext_id(idt,id,rtrbit) do{	\
		(idt).std.rb = 3; \
		(idt).std.rtr = ( (rtr) != 0); \
		(idt).std.ID = (id); \
	}while(0)
