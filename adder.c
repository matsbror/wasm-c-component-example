#include "adderw.h"

int32_t exports_wasmtime_wasi_adder_add(int32_t a, int32_t b){
    return a + b;
}

