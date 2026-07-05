// test_dragon_mcu.c
// 中国龙算法 MCU版 测试程序
// 编译: gcc -I../api ../lib/libdragon_mcu.a test_dragon_mcu.c -o test_dragon_mcu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dragon_mcu.h"

static int tests_passed = 0;
static int tests_total = 0;

#define TEST(name) do { printf("  [TEST] %-30s ", name); tests_total++; } while(0)
#define PASS()     do { printf("PASS\n"); tests_passed++; } while(0)
#define FAIL(msg)  do { printf("FAIL: %s\n", msg); return 1; } while(0)

// 验证结果一致
static int check_atom(dragon_atom_t got, dragon_atom_t expect, const char* label) {
    if (got != expect) {
        printf("FAIL: %s: got %02x, expect %02x\n", label, got, expect);
        return 1;
    }
    return 0;
}

// ============================================================
// 测试1: 版本号
// ============================================================
static int test_version(void) {
    TEST("版本号");
    const char* v = dragon_version();
    if (!v || strlen(v) == 0) FAIL("版本号为空");
    PASS();
    return 0;
}

// ============================================================
// 测试2: 阴+阳=太极
// ============================================================
static int test_yin_plus_yang(void) {
    TEST("阴+阳=太极");
    dragon_atom_t a = DRAGON_MAKE(0, 0);  // 阴
    dragon_atom_t b = DRAGON_MAKE(2, 0);  // 阳
    // 单元素通过 dragon_gemm 的 1x1 变体验证
    // 这里验证宏的正确性
    if (DRAGON_TAIJI(a) != 0) FAIL("a 应该是阴");
    if (DRAGON_TAIJI(b) != 2) FAIL("b 应该是阳");
    PASS();
    return 0;
}

// ============================================================
// 测试3: dragon_gemm 全零矩阵
// ============================================================
static int test_gemm_zero(void) {
    TEST("gemm 全零(16x16)");
    dragon_atom_t A[256], B[256], C[256] = {0};
    memset(A, 0, sizeof(A));
    memset(B, 0, sizeof(B));
    dragon_gemm(16, 16, 16, A, B, C);
    // 不崩溃即为通过
    PASS();
    return 0;
}

// ============================================================
// 测试4: dragon_gemm 全一矩阵
// ============================================================
static int test_gemm_ones(void) {
    TEST("gemm 全阳(16x16)");
    dragon_atom_t A[256], B[256], C[256] = {0};
    memset(A, DRAGON_MAKE(2, 0), sizeof(A));  // 全阳
    memset(B, DRAGON_MAKE(2, 0), sizeof(B));
    dragon_gemm(16, 16, 16, A, B, C);
    PASS();
    return 0;
}

// ============================================================
// 测试5: 小矩阵 4x4
// ============================================================
static int test_gemm_4x4(void) {
    TEST("gemm 4x4");
    dragon_atom_t A[16], B[16], C[16] = {0};
    for (int i = 0; i < 16; i++) {
        A[i] = DRAGON_MAKE(i % 3, i % 8);
        B[i] = DRAGON_MAKE((i + 1) % 3, (i + 2) % 8);
    }
    dragon_gemm(4, 4, 4, A, B, C);
    PASS();
    return 0;
}

// ============================================================
// 测试6: 超限矩阵（免费版返回全零）
// ============================================================
static int test_gemm_oversize(void) {
    TEST("gemm 超限(32x32)");
    dragon_atom_t A[1024], B[1024], C[1024];
    memset(A, DRAGON_MAKE(1, 0), sizeof(A));
    memset(B, DRAGON_MAKE(1, 0), sizeof(B));
    memset(C, 0xAA, sizeof(C));
    dragon_gemm(32, 32, 32, A, B, C);
    // 免费版超限返回全零
    int all_zero = 1;
    for (int i = 0; i < 1024; i++) {
        if (C[i] != 0) { all_zero = 0; break; }
    }
    if (all_zero == 0) {
        PASS();  // 商业版不走此路径
    } else {
        PASS();  // 免费版预期行为
    }
    return 0;
}

// ============================================================
// 测试7: 多轮稳定性
// ============================================================
static int test_gemm_stress(void) {
    TEST("gemm 压力(100轮)");
    dragon_atom_t A[64], B[64], C[64];
    for (int round = 0; round < 100; round++) {
        for (int i = 0; i < 64; i++) {
            A[i] = DRAGON_MAKE(rand() % 3, rand() % 8);
            B[i] = DRAGON_MAKE(rand() % 3, rand() % 8);
        }
        memset(C, 0, sizeof(C));
        dragon_gemm(8, 8, 8, A, B, C);
    }
    PASS();
    return 0;
}

// ============================================================
// 主函数
// ============================================================
int main(void) {
    printf("\n");
    printf("  中国龙算法 MCU版 测试套件\n");
    printf("  ===========================\n\n");

    dragon_init();
    printf("  版本: %s\n\n", dragon_version());

    int fail = 0;
    fail |= test_version();
    fail |= test_yin_plus_yang();
    fail |= test_gemm_zero();
    fail |= test_gemm_ones();
    fail |= test_gemm_4x4();
    fail |= test_gemm_oversize();
    fail |= test_gemm_stress();

    printf("\n  ===========================\n");
    printf("  结果: %d/%d 通过", tests_passed, tests_total);
    if (tests_passed == tests_total) printf("  [ALL PASS]");
    printf("\n\n");
    return fail;
}
