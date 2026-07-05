// stm32f103.c
// STM32F103C8T6 中国龙算法示例
// 编译: arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -I../api ../lib/libdragon_mcu.a stm32f103.c -o firmware.elf

#include "dragon_mcu.h"

#define MAT_SIZE 16

static const dragon_atom_t weights[MAT_SIZE * MAT_SIZE] = {0};
static dragon_atom_t input[MAT_SIZE * MAT_SIZE];
static dragon_atom_t output[MAT_SIZE * MAT_SIZE];

int main(void) {
    dragon_init();

    for (int i = 0; i < MAT_SIZE * MAT_SIZE; i++)
        input[i] = DRAGON_MAKE(i % 3, i % 8);

    dragon_gemm(MAT_SIZE, MAT_SIZE, MAT_SIZE, weights, input, output);

    while (1);
}
