# 中国龙算法 MCU版

全球首个 **5-bit混合态矩阵乘法引擎**，专为无FPU的嵌入式设备设计。

```
dragon_gemm(Cortex-M0) = 2.5x int8_gemm(Cortex-M4F)   实测@72MHz
```

## 核心卖点

| 特性 | 说明 |
|------|------|
| **零浮点** | 完全不依赖硬件FPU，Cortex-M0/RISC-V/8051 通吃 |
| **零分支** | 全部 9+27+24 种组合预计算查表，无流水线停顿 |
| **体积极小** | 静态库 < 16KB，RAM 运行时 < 512 字节 |
| **内存省** | 权重仅为 FP32 的 1/4，适合 Flash 受限芯片 |
| **时序安全** | 常数时间执行，无侧信道泄漏风险 |

## 快速开始

```bash
# 1. 克隆仓库
git clone https://github.com/xuantong/dragon-mcu.git
cd dragon-mcu

# 2. 将头文件和静态库加入你的项目
#    编译时链接：
gcc -I./api -L./lib -ldragon_mcu main.c -o firmware.elf

# 3. 在代码中调用
#include "dragon_mcu.h"

dragon_atom_t A[256], B[256], C[256];
dragon_init();
dragon_gemm(16, 16, 16, A, B, C);
```

## 示例目录

- `examples/stm32f103/` — STM32F103C8T6 标准库工程
- `examples/esp32/` — ESP32 ESP-IDF 工程

## 商业授权

本仓库内容按以下策略发布：

| 内容 | 协议 | 说明 |
|------|------|------|
| `api/dragon_mcu.h` | **Apache 2.0** | API 声明，可自由使用 |
| `examples/` | **Apache 2.0** | 示例代码，可直接运行 |
| `docs/` | **Apache 2.0** | 文档和性能数据 |
| `lib/libdragon_mcu.a` | **试用版** | 免费试用，16×16 矩阵限制 |
| 完整源码 | **商业授权** | 邮件联系玄同工作室 |

**免费版限制**：内置矩阵尺寸上限 16×16。超限矩阵乘法返回全零。

**解除限制**：商业授权版移除限制，提供源码 + 定制优化支持。

## 性能数据

| 场景 | 中国龙算法 | 传统 FP32 | 优势 |
|------|-----------|-----------|------|
| Cortex-M0 @48MHz | **0.8ms** (16×16) | 不可用 (无FPU) | N/A |
| Cortex-M4F @168MHz | **0.3ms** (16×16) | 0.5ms | **1.6x** |
| 内存占用 (16×16) | **256 字节** | 1024 字节 | **4x** |

详细数据见 `docs/benchmark.md`

## 联系方式

玄同工作室 — 455643142@qq.com

---

*中国龙算法 MCU版 v2.0 — 太极生八卦，算力出乾坤*
