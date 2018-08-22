#pragma once
#include <inttypes.h>

void InitModuleManager();

void ProcessModuleManager();

void SendToModule(uint8_t ir, uint8_t vis, uint8_t uv);
