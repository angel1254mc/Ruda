#pragma once
#include <stdio.h>
#include <string>
#include <threads.h>

struct util_once_flag {
   bool called;
   once_flag flag;
};

typedef struct simple_mtx_t {
   util_once_flag flag;
   mtx_t mtx;
} simple_mtx_t;

void print(std::string string);
void print(char* string);

