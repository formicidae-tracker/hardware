#pragma once


#if defined(__AVR_AT90CAN128__) \
	|| defined(__AVR_AT90CAN64__) \
	|| defined(__AVR_AT90CAN32__)
#define YACCL_MOB_MASK 0x0f
#elif defined (__AVR_ATmega16M1__) \
	||defined (__AVR_ATmega32M1__) \
	||defined (__AVR_ATmega64M1__)
#define YACCL_MOB_MASK 0x07
#else
#error "Unsuported MCU in yaccl-config.h"
#endif

#define YACCL_NB_MOB (YACCL_MOB_MASK-1)
