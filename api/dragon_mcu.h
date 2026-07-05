// dragon_mcu.h
// 中国龙算法 MCU精简版 v2.0
// 玄同工作室
//
// 零分支 · 零浮点 · 4x4分块 · 全查表
// 适用于 ARM Cortex-M / RISC-V / 8051 等无FPU嵌入式平台
// 编译为静态库发布，源码不公开

#ifndef DRAGON_MCU_H
#define DRAGON_MCU_H

#include <stdint.h>

typedef uint8_t dragon_atom_t;

#define DRAGON_TAIJI(a)  ((a) & 0x03)
#define DRAGON_BAGUA(a)  (((a) >> 2) & 0x07)
#define DRAGON_MAKE(t,b) (((b) << 2) | ((t) & 0x03))

void dragon_init(void);
const char* dragon_version(void);

void dragon_gemm(
    uint16_t m, uint16_t n, uint16_t k,
    const dragon_atom_t* __restrict a,
    const dragon_atom_t* __restrict b,
    dragon_atom_t* __restrict c
);

#endif
