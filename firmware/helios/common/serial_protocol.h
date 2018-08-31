#pragma once

#define MESSAGE_LENGTH 4
#define MESSAGE_START 0xff


#define VIS_POS 1
#define UV_POS 2
#define CS_POS 3

#define COMPUTE_CHECKSUM(buffer) ( (MESSAGE_START ^ buffer[VIS_POS]) ^ buffer[UV_POS])
