## 1. Resource Acquisition & Viewing Sequence Strategy

### 资源角色与检索方向

| 资源类型 | 在闭环中的角色 | 可靠获取方向（不编造链接） |
|---|---|---|
| **Lectures（视频/幻灯片）** | 建立概念模型的第一入口；Recitation 则直接服务于对应 Lab 的工具链与调试方法 | 官方日程页内含每讲的 pdf/pptx 与视频入口；视频亦常见于公开视频平台合集。搜索：`15213-f15 schedule.html` |
| **CS:APP 3e 教材** | 提供比 lecture 更完整的推导、边界情形与习题；Practice Problem 用于即时检验理解，Homework Problem 用于阶段收束 | 教材官网学生站提供各章补充材料与 GDB 速查表。搜索：`CS:APP3e student site` |
| **Lab Handout** | 唯一的“验收器”：所有学习最终必须收敛到可测试的 Lab 行为上 | 教材官网 labs 页面集中发放 handout；15-213 课程站也有对应学期的 lab 说明。搜索：`CS:APP3e labs datalab-handout.tar` |
| **Recitation** | 不是 lecture 的简单重复，而是**工具链注入**：GDB 断点/反汇编、`objdump`、`valgrind`、`make`、信号实验等 | 与 lecture 同页归档；重点关注 Data Lab、Bomb Lab、Malloc Lab、Proxy Lab 对应的 recitation |

### 单个 Topic 的闭环顺序

推荐每个知识单元按以下**五步循环**执行，不要先刷完所有视频再碰 Lab：

1. **Lecture 第一遍（1.2–1.5× 速度）**：只求抓住“这个机制解决什么问题、输入输出是什么”，不纠结汇编细节。
2. **教材对应章节精读**：读到 Lecture 未展开的边界条件时停下来；**每节做 2–3 道 Practice Problem**，尤其是带 ⭐ 的题。
3. **Lecture 第二遍 / Recitation**：带着教材中的疑问回看，重点关注**工具演示**（GDB 命令、`objdump -d`、`perf`、`valgrind` 用法）。
4. **进入 Lab 的“最小可运行版本”**：先让 Lab 的 driver/test 跑起来，哪怕只通过 1 个 case；再按知识模块逐个攻破。
5. **Lab 通过后回读教材 Homework Problem**：Homework 往往比 Practice 更接近“没有 driver 提示时你能否独立推出正确行为”，用于确认 mental model 是否真正稳固。

**关键原则**：Lab 的“入口时机”不是“Lecture 全部看完之后”，而是**前置依赖满足之后立刻进入**。例如 Bomb Lab 不需要等链接和缓存讲完，但必须在 Machine Prog: Procedures 之后开始。

---

## 2. Fine-Grained Knowledge Map & Progression Order

以下 7 个 Stage 严格按前置依赖排序。Lecture 编号与主题以 Fall 2015 官方日程为准；章节号对应 CS:APP 3e。

### Stage 1：数据表示——位、整数与浮点

**15-213 Fall 2015 Lectures**：L02–L04（Bits, Bytes, and Ints 两部分 + Floating Point）  
**CS:APP 3e**：Ch. 2（§2.1–§2.4），重点 §2.2 整数表示与转换、§2.3 整数运算、§2.4 浮点

**核心概念**：无符号与补码的位级等价性；截断与符号扩展；`x < 0` 与 `x <= 0` 在无符号语境下的失效；整数乘法/除法的移位实现与舍入方向；IEEE 754 的规格化数、非规格化数、特殊值（Inf/NaN）；舍入模式与浮点结合律失效。

**典型陷阱**：
- 有符号/无符号隐式转换导致的循环条件或比较错误（如 `sizeof` 返回 `size_t`）。
- 补码取负 `-x` 在 `INT_MIN` 上溢出（仍是 `INT_MIN`）。
- 浮点 `0.1 + 0.2 != 0.3` 被误认为“精度 bug”，而非理解二进制表示的有限性。
- 整数除法向零舍入，而算术右移向负无穷舍入，对负数结果不一致。

**Lab**：**Data Lab**  
**前置**：无需 Lab 前置；但必须先完成 §2.1–§2.3 的位运算与补码练习。  
**入口时机**：L03 结束后即可开始，L04 浮点部分结束后完成浮点函数。

---

### Stage 2：机器级表示与程序控制流（含攻击视角）

**15-213 Fall 2015 Lectures**：L05–L09（Machine Prog: Basics → Control → Procedures → Data → Advanced）  
**CS:APP 3e**：Ch. 3（§3.1–§3.10），其中 §3.7 过程调用是 Bomb Lab 与 Attack Lab 的共同分水岭

**核心概念**：x86-64 寄存器角色（`%rax` 返回值、`%rdi/%rsi/%rdx/%rcx/%r8/%r9` 参数、`%rbx/%rbp/%r12–%r15` callee-saved）；`mov` 族的后缀与操作数大小；`lea` 作为地址计算而非内存访问；条件码与 `cmp`/`test`；`jmp` 与条件跳转；`call`/`ret` 与栈帧；**16 字节栈对齐**；数组/结构体/联合体的地址计算；缓冲区溢出与 ROP 的基本图景。

**典型陷阱**：
- 把 `lea` 误读为“加载内存”；`lea 0x8(%rdi), %rax` 计算的是 `rdi + 8`，不解引用。
- 调用者保存与被调用者保存寄存器的混淆，导致过程调用后局部值被覆盖。
- `call` 前栈指针未按 16 字节对齐，在 `printf`/`sscanf` 等使用 SSE 的库函数处崩溃。
- 数组越界访问在汇编层面不一定立即报错，Bomb Lab 中常表现为读到“看似无关”的栈数据。
- 对 `mov` 源/目的操作数顺序的误判（AT&T 语法：`mov src, dst`）。

**Lab**：**Bomb Lab**（主）→ **Attack Lab**（延伸）  
**前置**：L05–L07 完成，尤其是过程调用与栈布局；GDB 的 `break`、`disas`、`x/`、`info registers` 必须已能独立使用。  
**入口时机**：L07 结束后开始 Bomb；L09 结束后开始 Attack。**不要跳过 Bomb 直接做 Attack**——Bomb 建立的“读汇编 → 推断行为 → 验证”的循环是 Attack 的前置技能。

---

### Stage 3：链接、优化与存储层次 / Cache

**15-213 Fall 2015 Lectures**：L10–L13（Code Optimization, Memory Hierarchy, Cache Memories, Linking）  
**CS:APP 3e**：Ch. 5（优化，选读 §5.1–§5.6 中与循环变换相关内容）、Ch. 6（§6.1–§6.7，缓存映射是核心）、Ch. 7（链接）

**核心概念**：
- **链接**：可重定位目标文件 `.o` 的节（`.text/.data/.bss/.rodata`）；符号表与 `static` 的局部化；强/弱符号解析规则；重定位条目（`R_X86_64_PC32` vs `R_X86_64_32`）；静态库的按需拉取与 `--whole-archive`；位置无关代码与 GOT/PLT。
- **存储层次与 Cache**：局部性原理；`(S, E, B, m)` 参数与地址分解为 `tag | set index | block offset`；直接映射、组相联、全相联；写回 vs 写直达；LRU 替换；**缓存冲突未命中**与容量未命中的区分；矩阵分块/循环重排对 miss rate 的影响。
- **优化**：CPE（每元素周期数）的测量；循环展开与多累积器；减少过程调用；避免不必要的内存引用。

**典型陷阱**：
- 缓存参数计算中把“块大小 B”与“总大小”混淆；set index 位数算错导致模拟器全 miss 或全 hit。
- Cache Lab Part A 中忽略 `I`（指令加载）与 `S`/`L`/`M` 的计数差异；`M` 视为一次 load + 一次 store。
- Part B 中只按行分块，没有考虑**列方向上的 set 冲突**；没有用 `valgrind` 的 cache 模拟验证。
- 链接时多个弱符号同名导致“选中的是意料之外的那个”；`.bss` 不占文件空间但运行时占内存，误以为“未初始化 = 不存在”。

**Lab**：**Cache Lab**（Part A: cache simulator；Part B: 矩阵转置优化）  
**前置**：L11–L12 完成，尤其是地址分解与组相联映射；C 语言指针与二维数组行/列遍历已熟练。  
**入口时机**：L12 结束后开始 Part A；Part B 需要 L10 的优化意识，可在 Part A 通过后立即进入。链接部分单独学习，不做强制 Lab。

---

### Stage 4：异常控制流与 Shell

**15-213 Fall 2015 Lectures**：L14–L16（ECF: Exceptions & Processes；ECF: Signals；System-Level I/O）  
**CS:APP 3e**：Ch. 8（§8.1–§8.8，重点异常分类、`fork`/`execve`/`waitpid`、信号语义、`sigprocmask`/`sigaction`）、Ch. 10（系统级 I/O：`open/read/write/dup2`、带缓冲 I/O 的陷阱）

**核心概念**：异常四分类（中断/陷阱/故障/终止）；`fork` 的写时复制与返回值语义；`execve` 替换地址空间而非“运行一个函数”；`waitpid` 的 `WNOHANG` 与 `WUNTRACED`；僵尸进程与回收；信号是**异步通知**而非排队信号量；`SIGCHLD` 默认忽略；信号处理函数中只能调用 async-signal-safe 函数；`sigprocmask` 阻塞关键区；`dup2` 实现重定向；`read` 返回 0 表示 EOF，不足 `n` 字节是正常现象。

**典型陷阱**：
- `fork` 后子进程继承了父进程的信号处理函数与阻塞掩码；父子竞态导致 `addjob`/`deletejob` 链表损坏。
- 在 `SIGCHLD` handler 中调用 `printf`（非 async-signal-safe）导致死锁或未定义行为。
- 误以为信号会“排队”：连续发送两个 `SIGCHLD`，handler 可能只执行一次。
- Shell Lab 中前台进程组与终端控制：没有 `tcsetpgrp` 会导致 `Ctrl-C` 发给 shell 自己而非前台作业。
- `write` 的输出少于预期时误以为出错，实际上内核可能部分写入，需要循环。

**Lab**：**Shell Lab**  
**前置**：L14–L16 完成；C 语言结构体链表与 `make` 已熟练。**Malloc Lab 之前必须先完成 Shell Lab**，因为 Shell 建立的进程/信号调试直觉是后续处理分配器内部状态的基础。  
**入口时机**：L16 结束后开始；若时间紧，至少要在 L15（Signals）后开始，I/O 部分在实现重定向时边做边补。

---

### Stage 5：虚拟内存与动态内存分配

**15-213 Fall 2015 Lectures**：L17–L20（VM: Concepts；VM: Systems；Storage Allocation: Basic；Storage Allocation: Advanced）  
**CS:APP 3e**：Ch. 9（§9.1–§9.11），重点 §9.9 分配器设计：隐式空闲链表、显式空闲链表、分离空闲链表、边界标记、合并、`realloc` 语义

**核心概念**：虚拟地址到物理地址的翻译；页表与 TLB；多级页表；`mmap` 与 `brk`；堆的起始与 `malloc` 返回的对齐要求（16 字节）；**隐式链表**的头部/脚部与边界标记；**显式链表**的 `prev/next` 指针；**分离空闲链表**的 size class 划分；首次适配/下一次适配/最佳适配；合并的四种情况（前空/后空/双空/均不空）；`realloc` 的就地扩展与搬迁；碎片（内部碎片 vs 外部碎片）的度量与权衡。

**典型陷阱**：
- 边界标记写错导致 `free` 时合并越界，`mmap` 检查报 “invalid pointer”。
- 忽略**最小块大小**（通常 16 字节）与对齐要求，返回的指针未 16 字节对齐，在 `printf`/`movaps` 处崩溃。
- 显式链表插入时没有正确更新四个指针（前后块的 `prev/next` + 自身的 `prev/next`），造成空闲链表断裂。
- `realloc` 缩小块时忘记处理剩余部分；扩大时盲目搬迁而不尝试与后继空闲块合并。
- 只追求利用率，忽略吞吐量；或只追求吞吐量，导致外部碎片爆炸。Malloc Lab 的评分是**两者的加权**，不是单项越高越好。

**Lab**：**Malloc Lab**  
**前置**：Stage 4 完成；Ch. 9 §9.9 精读；GDB 已能用于检查堆内存。**这是全课程最耗时的 Lab**，自学常见耗时 20–40 小时。  
**入口时机**：L20 结束后开始基础实现；分离空闲链表和 `realloc` 优化可在有了可工作的隐式/显式版本后迭代进入。**不要在 VM 概念尚未理解时直接写分配器**——你会在 `brk` 与堆边界上反复撞墙。

---

### Stage 6：网络编程与并发

**15-213 Fall 2015 Lectures**：L21–L23+（Network Programming: Part 1/2；Concurrent Programming；同步与线程安全）  
**CS:APP 3e**：Ch. 11（§11.1–§11.6：套接字接口、`getaddrinfo`、客户端-服务器模型、HTTP 基础）、Ch. 12（§12.1–§12.7：线程、互斥锁、条件变量、信号量、并发服务器设计、线程安全与可重入性）

**核心概念**：`socket`/`bind`/`listen`/`accept`/`connect`；`getaddrinfo` 与协议无关编程；`rio` 带缓冲 I/O；HTTP 请求行/头/体解析；`Host` 头与代理场景下的完整 URL 转换；**基于线程的并发**（`pthread_create`，detached 线程）；互斥锁保护共享缓存；条件变量与生产者-消费者；**预线程化**与每连接一线程的取舍；`select`/`epoll` 的事件驱动模型概览。

**典型陷阱**：
- Proxy Lab 中不转发 `Host` / `Connection` / `Proxy-Connection`，导致目标服务器返回错误或连接语义错误。
- 用一把全局大锁保护整个缓存，虽然正确但无法通过 Proxy Lab 的并发性能测试；需要**分段锁**或**读写锁**。
- 忽略 `Content-Length` 的精确转发，或对 chunked encoding 处理错误。
- 线程未 detach，导致僵尸线程耗尽资源。
- 在信号处理函数中做网络 I/O；或在多线程中混用 `signal` 与 `sigwait` 时语义错误。

**Lab**：**Proxy Lab**  
**前置**：Stage 4（系统 I/O + 进程/信号）+ Stage 6 网络 lecture；Shell Lab 建立的“并发正确性”直觉直接迁移。  
**入口时机**：Network Part 1 结束后先实现单线程代理并通过基础测试；Concurrent Programming 结束后加入缓存与线程，完成最终版本。

---

## 3. Module-Specific Study Methods

### 3.1 位级 / 数据表示（Data Lab 型）

**阅读**：§2.1–§2.4 不要跳读“补码的位级等价性”。每读完一个小节，合上书，用纸笔写出该小节对应的 2–3 个边界表达式（如 `INT_MIN` 取负、`-1` 转无符号、非规格化浮点的最大值）。

**编码验证**：Data Lab 的 `dlc` 检查器会拒绝常量超出 `0x00–0xFF` 或非法运算符。先写“逻辑正确但可能超常量”的版本，再用 **掩码 + 移位** 压缩常量。每一步用 `printf("%x")` 打印中间位模式，与手工推演对照。

**调试**：如果 `dlc` 通过但测试失败，优先怀疑**有符号/无符号转换点**和**移位量 ≥ 位宽**。对浮点题，用 `union { float f; unsigned u; }` 查看位级表示，不要用 `printf("%f")` 猜。

### 3.2 汇编 / 逆向 / 控制流（Bomb + Attack 型）

**阅读**：§3.7 过程调用和 §3.10 越界/栈布局必须精读。读汇编时**始终在纸面维护两个栈帧图**：调用者的栈帧（返回地址、保存的寄存器）和被调用者的栈帧（局部变量、参数溢出区）。

**逆向工作流**：
1. 对 Bomb 的每个 phase，先用 `objdump -d` 定位 `phase_n` 和其调用的辅助函数。
2. 从 `call` 前的寄存器设置反推期望输入格式（`sscanf` 的格式串是金矿）。
3. 用 GDB `b phase_n` → `ni`/`si` 单步，每步后 `info registers rax rdi rsi`，记录比较指令两侧的值。
4. **不要试图在脑中模拟整个程序**；只追踪从输入到 `explode_bomb` 之间的数据依赖切片。

**Attack 调试**：构造 payload 前先在 GDB 中确认目标函数的栈帧布局和 `%rsp` 值。`getbuf` 返回时的 `ret` 弹出的是你覆盖的返回地址；若程序在 `movaps` 处崩溃，首先检查**栈对齐**（`%rsp % 16 == 0` 在 `call` 之前）。

### 3.3 缓存 / 存储层次（Cache Lab 型）

**阅读**：§6.4 的地址分解必须能手算。给定 `(S, E, B, m)`，对任意地址写出 `tag`、`set`、`offset` 的位数和值域。

**Part A 工作流**：先用 `valgrind --tool=lackey --trace-mem=yes` 观察真实 trace 格式；实现 `cache_access(addr, type)` 时，**把 `I` 和 `S/L/M` 分开计数**，`M` 拆成一次 `L` + 一次 `S`。用课程提供的 `test-csim` 和 `test-trans` 逐项对照，不要等全部写完再测。

**Part B 工作流**：32×32 和 64×64 的目标 miss 数不同。先测量 baseline（朴素转置）的 miss 数，再以 **8×8 分块**为起点，用 `./csim-ref -v` 观察每个 set 的冲突模式。对于 64×64，重点关注**每 4 行映射到同一 set 组**导致的冲突；用**分块内重排 + 暂存局部变量**打破冲突，不要盲目增大块尺寸。

### 3.4 异常控制流 / Shell（Shell Lab 型）

**阅读**：§8.5 信号语义必须区分“pending”与“blocked”；§8.4 的 `waitpid` 选项要逐项理解。

**编码工作流**：Shell Lab 的核心不是“解析命令行”，而是**作业控制的状态机**。先画出三张状态转移：前台作业、后台作业、停止作业。`addjob`/`deletejob`/`waitfg` 的调用点必须与信号 handler 中的 `deletejob` 严格互斥。

**调试**：
- 用 `trace` 文件驱动，**逐个 trace 通过**，不要一次跑全套。
- 在 `SIGCHLD` handler 中 `printf` 是常见错误；改用 `write(STDERR_FILENO, ...)` 或 GDB `dprintf`。
- 竞态问题用 `valgrind --tool=helgrind` 辅助定位；`sigprocmask` 包围 `fork` + `addjob` 是关键临界区。

### 3.5 动态内存分配（Malloc Lab 型）

**阅读**：§9.9 的三代分配器（隐式 → 显式 → 分离）必须按顺序理解。不要一上来就写分离链表；先用隐式链表通过 `trace` 的正确性，再优化。

**编码 / 调试工作流**：
1. **先写 `mm_init` 和最小 `malloc/free`**，让 `mdriver` 能跑完一个小 trace 而不崩溃。
2. 用 `mdriver -V` 观察每个 trace 的 `util` 和 `thru`；**正确性优先于性能**：先让所有 trace 的 `valid` 通过，再优化。
3. 边界标记错误用 `mdriver -f` 单 trace 调试；在 `free` 中 `assert` 前后块的 `alloc` 位一致。
4. 合并逻辑的四种情况分别写单元测试思路（在纸上推演指针变化），再合入代码。
5. `realloc` 的缩小分支容易忽略；**缩小剩余部分必须成为合法空闲块并尝试合并**。

### 3.6 网络 / 并发（Proxy Lab 型）

**阅读**：§11.4 `getaddrinfo` 和 §11.6 HTTP 报文格式；§12.3 互斥锁、§12.5 条件变量、§12.6 并发服务器。

**编码工作流**：
1. **先实现阻塞式单线程代理**，用 `curl -x localhost:port` 验证 GET 转发和响应回传。
2. 加入**完整 URL → 路径 + Host 头**的转换；用 `telnet` 手工构造请求，观察字节级转发。
3. 单线程稳定后，加入**每个连接一线程**（detach），再引入缓存。
4. 缓存使用**分段锁**（如 8–16 个桶），每桶一把互斥锁；LRU 更新在锁内完成。

**调试**：Proxy Lab 不要用浏览器测缓存（浏览器自身缓存会干扰）；用 `curl -v` 和课程提供的 `driver.sh`。代理进程崩溃后用 GDB `attach` 到线程，或开启 core dump 后 `gdb ./proxy core`。

---

## 4. Phased Study Plan & Milestone Acceptance Criteria

**假设**：Personal Parameters 保持默认值——C/C++ 语法与指针基础、基本 Linux CLI、无系统背景；每周 10–12 小时（工作日 1.5 h × 5，周末 3 h × 2）；目标包含面试准备。以下按 **14 周** 规划，Stage 5（Malloc Lab）预留最多时间，因为它是公认的瓶颈。

### Phase 1（Week 1–2）：数据表示 + Data Lab

| 项 | 内容 |
|---|---|
| **时间分配** | Week 1：L02–L04 + §2.1–§2.4；Week 2：Data Lab 主要攻坚 + Practice/Homework 选做 |
| **单次节奏** | 工作日 1.5 h：40 min lecture + 50 min 教材/Practice；周末 3 h：连续 Lab 编码 + GDB 验证 |
| **交付物** | `bits.c` 通过 `dlc` 且 `btest` 全部通过；`fshow`/浮点部分通过 |
| **验收标准** | 能脱离教材解释：`x < 0` 在无符号下为何失效；`-INT_MIN` 为何仍是 `INT_MIN`；`(float)INT_MAX` 舍入方向；能用位运算手写 `isPower2` 和 `float_f2i` 的边界处理 |

### Phase 2（Week 3–4）：机器级表示 + Bomb Lab + Attack Lab

| 项 | 内容 |
|---|---|
| **时间分配** | Week 3：L05–L08 + §3.1–§3.9，同步开始 Bomb 前 3 个 phase；Week 4：Bomb 收尾 + L09 + Attack |
| **单次节奏** | 工作日：1 h lecture + 30 min 汇编阅读/`objdump` 练习；周末：3 h 整块 GDB 逆向 |
| **交付物** | Bomb 全部 6 个 phase（含隐藏 phase 选做）；Attack 至少完成 `touch1`–`touch3` 与一个 ROP 阶段 |
| **验收标准** | 能在 GDB 中不依赖源码，仅凭 `disas` 和 `info registers` 追踪一个函数调用的参数传递与栈帧变化；能解释 16 字节对齐对 `movaps` 崩溃的影响；能手工构造覆盖返回地址的 payload |

### Phase 3（Week 5–6）：链接 + 缓存 + Cache Lab

| 项 | 内容 |
|---|---|
| **时间分配** | Week 5：L10–L12 + §6.1–§6.7，Cache Part A 同步启动；Week 6：Part A 收尾 + Part B + 链接 §7 选读 |
| **单次节奏** | 工作日：1 h lecture + 30 min 缓存参数手算/Practice；周末：3 h Part B 优化 + `valgrind` 对照 |
| **交付物** | `csim` 通过全部 `test-csim`；`trans` 的 32×32 和 64×64 达到课程 benchmark 的 miss 数阈值 |
| **验收标准** | 给定任意地址和 `(S,E,B,m)` 能在 60 秒内正确分解 tag/set/offset；能解释“为什么 64×64 的冲突未命中在朴素分块下比 32×32 更严重”；能用 `valgrind --tool=lackey` 生成 trace 并手工验证 simulator 的计数 |

### Phase 4（Week 7–8）：异常控制流 + 系统 I/O + Shell Lab

| 项 | 内容 |
|---|---|
| **时间分配** | Week 7：L14–L16 + §8.1–§8.8 + §10；Week 8：Shell Lab 全量 trace 通过 |
| **单次节奏** | 工作日：lecture + 信号语义手写笔记；周末：3 h trace 驱动调试 + `helgrind` |
| **交付物** | `tsh` 通过课程提供的全部 trace（包括 `trace16`–`trace18` 等信号/作业控制用例） |
| **验收标准** | 能解释 `SIGCHLD` 不排队导致的经典竞态，并说明 `sigprocmask` 为什么必须包住 `fork` + `addjob`；能指出在 handler 中使用 `printf` 的具体风险；能画出前台/后台/停止作业的状态机 |

### Phase 5（Week 9–11）：虚拟内存 + Malloc Lab（**最长阶段**）

| 项 | 内容 |
|---|---|
| **时间分配** | Week 9：L17–L20 + §9.1–§9.11，写隐式链表并通过小 trace；Week 10：显式/分离链表优化 + `realloc`；Week 11：性能调优 + 全部 trace 稳定性 |
| **单次节奏** | 工作日以阅读和设计为主，周末用完整的 3 h 块做“实现 → `mdriver` → GDB”循环；**Malloc 不适合碎片化时间硬推** |
| **交付物** | `mm.c` 通过 `mdriver` 全部 trace 的 `valid` 检查；`util` 和 `thru` 达到课程基准的合理区间（自学目标：`util` ≥ 50%，`thru` 不为零且稳定） |
| **验收标准** | 能解释边界标记在合并中的作用；能手工推演四种合并情况下四个指针的正确更新顺序；能说明“最小块大小 16 字节”与“返回指针对齐”之间的关系；能识别并修复 `free` 时前后块 `alloc` 位不一致导致的 `mdriver` 崩溃 |

### Phase 6（Week 12–14）：网络 + 并发 + Proxy Lab + 面试收束

| 项 | 内容 |
|---|---|
| **时间分配** | Week 12：Network Part 1/2 + §11，实现单线程代理；Week 13：Concurrent + §12，加入缓存与线程；Week 14：Proxy 收尾 + 全课程 interview 主题复盘 |
| **单次节奏** | 工作日：lecture + 套接字/HTTP 字节级验证；周末：3 h `driver.sh` + 并发压测 + GDB attach |
| **交付物** | Proxy 通过基础 `driver.sh` 测试；缓存使用分段锁而非全局大锁；线程 detach |
| **验收标准** | 能解释 `getaddrinfo` 返回链表的使用方式；能指出代理转发中 `Host` 头与完整 URL 的关系；能说明为什么“一把大锁”不可接受，并描述分段锁/读写锁的取舍；能独立复述以下面试高频链路：**栈帧与过程调用 → 缓存映射与局部性 → 符号解析/重定位 → 虚拟内存/分配器 → 信号竞态 → 并发服务器模型** |

**里程碑总览**：Phase 1 通过 Data Lab 证明你建立了正确的**位级 mental model**；Phase 2 通过 Bomb/Attack 证明你建立了**汇编级控制流与栈模型**；Phase 3 通过 Cache 证明你建立了**存储层次与地址分解模型**；Phase 4 通过 Shell 证明你建立了**进程/信号/并发正确性模型**；Phase 5 通过 Malloc 证明你理解了**堆的内部表示与空间/时间权衡**；Phase 6 通过 Proxy 将上述所有模型收敛到**真实系统编程与并发工程**。如果某一 Phase 的验收标准未通过，**不要带着漏洞进入下一阶段**——后续 Lab 会以更隐蔽的方式暴露同一类模型缺失。
