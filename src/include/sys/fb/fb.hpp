#pragma once
#include <stdint.h>
#include <stddef.h>
#include <init/kinfo.hpp>

void InitFB();
void FB_PrintScreen(const char *string);
void FB_PutChar(const char ch);
void FB_PrintScreen(int row, int col, const char *string);

