# 中国龙算法 MCU版 API文档

## 概述

中国龙算法 MCU版提供纯整数5-bit混合态矩阵乘法引擎，专为无FPU的嵌入式设备设计。全部算术操作通过预计算查表实现，零分支、零浮点。

## 数据类型

### `dragon_atom_t` — 混合态原子 (8-bit)

```c
typedef uint8_t dragon_atom_t;
```

位布局:
- Bit[1:0] — 太极数值 (0=阴, 1=太极, 2=阳)
- Bit[4:2] — 八卦拓扑态 (0=坤 ~ 7=乾)
- Bit[7:5] — 保留

### 字段提取宏

| 宏 | 功能 |
|----|------|
| `DRAGON_TAIJI(a)` | 提取太极值 |
| `DRAGON_BAGUA(a)` | 提取八卦态 |
| `DRAGON_MAKE(t,b)` | 构造原子 |

## 函数参考

### `dragon_init`

```c
void dragon_init(void);
```

初始化算法引擎。当前为空操作，预留用于硬件自检。应在系统启动后最先调用。

### `dragon_version`

```c
const char* dragon_version(void);
```

返回版本号字符串，如 `"中国龙算法 MCU v2.0"`。

### `dragon_gemm`

```c
void dragon_gemm(
    uint16_t m, uint16_t n, uint16_t k,
    const dragon_atom_t* __restrict a,
    const dragon_atom_t* __restrict b,
    dragon_atom_t* __restrict c
);
```

混合态矩阵乘法：C[M][N] += A[M][K] × B[K][N]

**参数:**
- `m, n, k` — 矩阵维度
- `a` — A矩阵 (行优先, 尺寸 m×k)
- `b` — B矩阵 (行优先, 尺寸 k×n)
- `c` — C矩阵 (行优先, 尺寸 m×n)

**约束:**
- 所有指针必须 `__restrict` (互不重叠)
- 建议 4 字节对齐

**免费版限制:** m, n, k ≤ 16。超限时返回全零。

## 内存需求

| 矩阵尺寸 | 原子数 | 字节 |
|---------|--------|------|
| 4×4 | 48 | 48 |
| 8×8 | 192 | 192 |
| 16×16 | 768 | 768 |
| 32×32 | 3072 | 3072 |

运行时额外栈需求: < 256 字节 (4×4累加器 + 缓存行)

## 平台兼容性

| 平台 | 状态 | 备注 |
|------|------|------|
| ARM Cortex-M0/M0+ | ✅ | 无需 FPU |
| ARM Cortex-M3/M4 | ✅ | 无需 FPU |
| ARM Cortex-M7 | ✅ | 兼容 |
| RISC-V RV32 | ✅ | 已验证 |
| ESP32 (Xtensa) | ✅ | ESP-IDF 示例 |
| 8051 | ✅ | 需调整 `uint16_t` |
