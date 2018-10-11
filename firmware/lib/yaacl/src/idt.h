#pragma once

#include <inttypes.h>
#include <stdbool.h>

#define YAACL_STD_ID_MSK (((1L << 11) - 1) & 0xffffffff)
#define YAACL_EXT_ID_MSK (((1L << 29) - 1) & 0xffffffff)
#define YAACL_IDEBIT_MSK ((1L << 29) & 0xffffffff)
#define YAACL_RTRBIT_MSK ((1L << 30) & 0xffffffff)

typedef uint32_t yaacl_idt_t;

#define yaacl_idt_is_ext(idt) ( ((idt) & YAACL_IDEBIT_MSK) != 0 )



#define yaacl_make_std_idt(idt,id,rtrbit) do{	  \
		(idt) = ((id) & YAACL_STD_IDT_MSK) \
			| ( (rtrbit != 0 ) ? YAACL_RTRBIT_MSK : 0 ); \
	}while(0)

#define yaacl_make_ext_idt(idt,id,rtrbit) do{	  \
		(idt) = ((id) & YAACL_EXT_IDT_MSK) \
			| ( (rtrbit != 0 ) ? YAACL_RTRBIT_MSK : 0 ) \
			| YAACL_IDEBIT_MSK; \
	}while(0)
