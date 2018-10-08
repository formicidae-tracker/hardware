#pragma once

#include <inttypes.h>
#include <stdbool.h>

struct s_yaacl_idt_std_t {
	bool rb0:1;
	bool reserved0:1;
	bool rtr:1;
	uint32_t reserved1:18;
	uint16_t ID:11;
} __attribute__((packed)) ;
typedef struct s_yaacl_idt_std_t yaacl_idt_std_t;

struct s_yaacl_idt_ext_t {
	uint8_t rb:2;
	bool rtr:1;
	uint32_t ID:29;
} __attribute__((packed));
typedef struct s_yaacl_idt_ext_t yaacl_idt_ext_t;

typedef union {
	uint32_t as_int_32;
	struct s_yaacl_idt_std_t std;
	struct s_yaacl_idt_ext_t ext;
	uint8_t data[4];
} yaacl_idt_t;



#define yaacl_idt_is_ext(idt) ( (idt).std.rb0 == 1 )

#define yaacl_make_std_idt(idt,id,rtrbit) do{	\
		(idt).std.rb0 = 0; \
		(idt).std.rtr = ( (rtr) != 0); \
		(idt).reserved0 = 0; \
		(idt).reserved1 = 0; \
		(idt).std.ID = (id); \
	}while(0)

#define yaacl_make_ext_idt(idt,id,rtrbit) do{	\
		(idt).std.rb = 3; \
		(idt).std.rtr = ( (rtr) != 0); \
		(idt).std.ID = (id); \
	}while(0)


#define yaacl_make_std_mask(msk,id,idebit,rtrbit) do{	  \
		(msk).std.rb0 = idebit; \
		(msk).std.rtr = ( (rtr) != 0); \
		(msk).reserved0 = 0; \
		(msk).reserved1 = 0; \
		(msk).std.ID = (id); \
	}while(0)

#define yaacl_make_ext_mask(msk,id,idebit,rtrbit) do{	  \
		(msk).std.rb = (idebit != 0 ? 0x1 : 0x0); \
		(msk).std.rtr = ( (rtr) != 0); \
		(msk).std.ID = (id); \
	}while(0)
