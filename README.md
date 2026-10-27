# STM32 Learning Notes

跟着江协科技 STM32 教程（BV1th411z7sn）从零学习嵌入式开发的实践代码仓库。
从点亮第一个 LED，到独立完成环境监测综合项目，这里记录了完整的学习轨迹。

> Practice code for learning STM32 from scratch. From blinking the first LED to building a
> complete environment monitoring system — the whole learning path is recorded here.

---

## 硬件与工具

| 项目 | 型号 / 版本 |
|---|---|
| 主控 | STM32F103C8T6（Cortex-M3，72 MHz） |
| 开发板 | 最小系统板 |
| 开发环境 | Keil µVision MDK-ARM |
| 外设库 | STM32F10x Standard Peripheral Library V3.5.0 |
| 编译器 | Arm Compiler 5（AC5） |
| 下载调试 | ST-Link |

**两个必须知道的坑：**

1. 本仓库全部基于**标准外设库（StdPeriph）**，不是 HAL 库。江协教程用的是标准库，寄存器操作更直观，适合打基础。
2. MDK 5.37 及以上版本**不再自带 AC5**，需要手动补装，装完还要在 Options for Target → Target 里把编译器切到 **V5**，否则会报一堆编译错误。
3. 板载 LED 是**低电平点亮**：输出 0 亮，输出 1 灭。

---

## 工程一览

| 目录 | 主题 | 涉及外设 / 知识点 |
|---|---|---|
| `1-1 project document template` | 工程模板 | 标准库工程结构、启动文件与 CMSIS 配置 |
| `2-1 LED glint` | LED 闪烁 | GPIO 输出配置、软件延时 |
| `2-2 LED waterfull light` | LED 流水灯 | GPIO 端口批量操作、数组轮询 |
| `2-3 buzzer` | 蜂鸣器 | 有源蜂鸣器驱动 |
| `2-4 key control` | 按键控制 LED | GPIO 上拉输入、按键消抖、读引脚电平 |
| `2-5 light control Buzzer` | 光敏电阻控制蜂鸣器 | GPIO 输入判断、模块化分层（Hardware/System/User） |
| `3-1 OLED screen` | OLED 显示屏 | OLED 驱动移植、字模取模、软件模拟通信时序 |
| `3-2 counter-acting infrared sensor count` | 红外对射传感器计数 | 外部中断 EXTI、NVIC 优先级、中断服务函数 |
| `3-3 Rotating encoded counter` | 旋转编码器计数 | 双相正交信号解码、旋转方向判断 |
| `species task environmental monitoring` | **综合项目：环境监测** | ADC 采集光照、DHT11 温湿度单总线、编码器调阈值、OLED 数据显示、SysTick 精确定时 |

---

## 目录结构

每个工程内部结构一致（江协风格的分层）：

```
<工程名>/
├── Hardware/         外设驱动模块（LED / Key / OLED / DHT11 ...）
├── System/           系统级功能（Delay 延时等）
├── Library/          标准外设库副本
├── start/            启动文件与 CMSIS 核心文件
├── user/             main.c、中断服务函数、库配置文件
└── project.uvprojx   Keil 工程文件
```

`Hardware/` 和 `System/` 是自己写的驱动代码；`Library/` 和 `start/` 是库文件副本，
**刻意保留**——这样 clone 下来不用再配环境，双击工程文件就能打开。

---

## 怎么跑起来

1. 用 **MDK-ARM** 打开任意工程目录下的 `project.uvprojx`
2. `Options for Target` → `Target` 选项卡，确认编译器是 **V5（AC5）**
3. 接上 ST-Link，编译 → 下载

> 51 单片机的工程是 `.uvproj`，要用 C51 的 µVision 打开；
> STM32 的工程是 `.uvprojx`，要用 MDK 的 µVision 打开。用错版本会报 `Cannot read project file`。

---

## 学习记录

`STM32_notes/` 下是学习过程中真实踩过的坑：

- 工程搭建与踩坑记录
- 2-1 LED 工程：从 26 个编译报错到烧录成功

比起整理好的知识点，这些记录了**当时是怎么错的、怎么排查的**，回头看更有用。

---

## 说明

- 这是跟着教程做的练习代码，不是生产级项目，但每个工程都能编译、能烧录、能跑
- 编译产物（`Objects/`、`.hex`、`.axf`、`.dep` 等）已通过 `.gitignore` 排除，仓库里只留源码
- 仓库已配置 `.gitattributes`，关闭了 git 的行尾自动转换，保证工程文件字节一致
- 持续更新中
