# C++ Primer 第5版 细粒度阅读路线（已完成 Frank Mitropoulos《Beginning C++ Programming - From Beginner to Beyond》）

> 
> 前置说明：Frank 的课是**视频驱动、案例导向、碎片化知识点**，覆盖：流程控制、函数、指针、类、构造/析构、拷贝/移动、运算符重载、继承多态、智能指针、vector、IO流、基础异常、STL入门；优势是上手写代码，短板是**标准库体系、类型系统细节、表达式求值、初始化规则、迭代器模型、模板、const/引用语义、底层陷阱、术语严谨性**薄弱，正好是 Primer 补齐的重点。
> 本书是 C++11，和课程的 C++14/17 兼容，读完 Primer 后再补新标准特性。
> 策略：**不是逐页从头慢啃，分「必精读 / 快速过 / 暂时略读」三级，每一章附带【你的盲区重点】+【课后题要求】+【校验目标】**

## 阅读总原则（先看这个）

1. 必做：**带书里的代码亲手敲，不要只看；标记你在课程里听过但讲得很浅的概念**
2. 课后题分级：A类（必做，基础校验）、B类（推荐，加深理解）、C类（可选，偏拓展，第一遍跳过）
3. 标记符号约定
   - ✅：你课程已经覆盖，快速浏览，只抓差异和严谨定义
   - ⭐：**核心盲区，精读，重点攻克（面试高频）**
   - 🟡：了解即可，第一遍浅读，后续遇到再回来深挖
4. 周期预估：每天 1.5–2h，整体 35–45 天完成第一遍精读；读完后可以无缝衔接 James Raynard 《Modern C++: From Intermediate to Advanced》

---

# Roadmap 正文（按 Primer 章节）

## 第1章 开始（Getting Started）

- 内容：简单程序、IO、注释、控制流、类初探、书店示例
- 你的现状：✅ IO、if/for 循环、简单类课程都写过
- 阅读策略：快速过，**重点看书店完整示例的设计思路**（不是语法，是程序结构）
- 重点留意：`using namespace std` 的利弊，不要养成无脑写的习惯
- 习题：A类必做，B类可选
- 读完校验：看懂一个小型完整程序的组织方式

## Part I 基础（The Basics）

### Ch2 变量和基本类型（Variables and Basic Types）⭐【超高优先级，重点盲区】

> 
> Frank 课只教怎么用变量，没有深挖类型体系、复合类型、顶层/底层 const

- 精读小节：
  - 内置类型：有符号/无符号、溢出陷阱、类型转换（隐式转换大坑）
  - 变量：声明 vs **定义（declaration / definition，面试必问）**
  - 复合类型：引用、指针；**引用绑定规则，左值引用不能绑定右值（课程移动语义那里接触过，但没有系统区分）**
  - `const`：**顶层 const / 底层 const（重中之重，课程几乎没讲透）**
  - `constexpr` 常量表达式（课程浅尝辄止）
  - 自定义数据结构 `struct`
- 快速跳过：简单变量初始化的基础例子
- 习题：A全部必做；B必做，专门练 const、指针、引用混合
- 读完校验：
  - 能区分：`const int *p` / `int *const p` / `const int &r`
  - 理解：什么时候隐式类型转换会出 bug（unsigned 负数比较）

### Ch3 string、vector、数组（Strings, Vectors, and Arrays）⭐

> 
> Frank 大量用 vector，但数组、多维数组、迭代器的底层模型讲得很薄

- 精读小节：
  - `std::string`：各种初始化、`substr`、`find`、比较规则、c_str()
  - `vector`：容器本质、增长策略（capacity vs size，课程没细讲扩容）
  - **迭代器基础：iterator、const_iterator，迭代器失效（非常高频坑）**
  - 内置数组：和 vector 的区别、数组到指针的隐式转换、指针就是数组迭代器
  - 多维数组：本质是数组的数组，不是二维矩阵
- 快速浏览：简单范围for遍历
- 习题：A必做；迭代器失效相关题目 B必做
- 读完校验：能说出 vector 扩容机制、什么时候迭代器失效、数组退化问题

### Ch4 表达式（Expressions）⭐【课程最大短板之一！】

> 
> Frank 只教运算符怎么写，不讲求值顺序、优先级、值类别、副作用

- 精读小节：
  - 基础运算符、左值 / 右值（lvalue / rvalue，为后面移动语义打牢理论）
  - 求值顺序、运算符优先级、短路求值 && ||
  - 类型转换：隐式转换、显式强制转换、`static_cast / const_cast`
  - 逗号运算符、条件运算符
- 跳过：特别冷门运算符
- 习题：强烈建议多做，这一章是很多隐形 bug 的来源
- 读完校验：能区分一个表达式是左值还是右值；看懂 `++i` 和 `i++` 表达式值类别差异

### Ch5 语句（Statements）✅

- 内容：if、switch、while、for、do while、break continue、try-catch 异常基础
- 你的现状：课程全部写过大量案例
- 策略：快速通读，只抓 switch 的 case 穿透、范围 for 的限制、异常抛出栈展开基础概念
- 习题：少量 A 类即可，不用耗大量时间

### Ch6 函数（Functions）⭐

> 
> Frank 讲了函数、重载，但没有系统梳理参数传递、返回、作用域、默认实参

- 精读小节：
  - 函数基础、**值传递 / 引用传递 / const引用传递（什么时候选哪个，工程习惯）**
  - 形参、默认实参、函数重载（重载决议规则！难点）
  - 返回值：返回引用的风险（返回局部变量引用，悬垂引用大坑）
  - 函数声明、头文件和源文件分离、独立编译模型
- 🟡：内联 inline、constexpr 函数先理解概念，不钻太深
- 习题：A+B必做，重点练重载决议、返回引用陷阱
- 读完校验：能说出：为什么优先 const& 传参；什么时候不能返回局部引用

### Ch7 类（Classes）⭐【OOP 体系系统化，课程 OOP 是零散案例】

> 
> Frank 写过类、构造、拷贝、移动、继承，但没有从「抽象数据类型 ADT」角度讲设计

- 精读小节：
  - 成员、访问控制 public/private、友元 friend（面试高频）
  - 构造函数、**默认构造、初始化列表（成员初始化顺序！致命坑，课程很少强调）**
  - 拷贝构造、拷贝赋值、析构（三法则 Rule of Three；延伸理解五法则 Rule of Five，对接你学的移动语义）
  - `this` 指针、const 成员函数（`const` 对象只能调用 const 成员函数，重点）
  - 静态成员 static / 静态成员函数
- 🟡：类的前向声明、mutable 先看懂概念
- 习题：A+B 全部重点做；自己写一个简单类，刻意踩初始化顺序、const成员函数的坑
- 读完校验：
  - 清楚三/五法则什么时候需要自己写拷贝/移动/析构
  - 区分：const成员函数里哪些能改、哪些不能改，mutable 的作用

## Part II 标准库（The C++ Standard Library）

### Ch8 IO 库（The IO Library）✅

> 
> Frank 讲 cin/cout、文件流，案例够用，Primer 用来补齐细节和错误状态

- 精读：IO 状态位 `eofbit/failbit/badbit`、流的条件判断、文件流模式
- 快速浏览：格式化输出
- 目标：理解流失败之后的状态恢复，避免 while(cin >> x) 踩坑

### Ch9 顺序容器（Sequential Containers）⭐

> 
> Frank 只重点讲 vector；deque / list / forward_list / array 只是提一嘴

- 精读小节：
  - 所有顺序容器对比：vector / deque / list / forward_list / array 各自性能、适用场景（面试高频对比）
  - 容器通用操作、插入删除对迭代器的影响
  - emplace 原位构造 vs push_back（拷贝/移动，现代C++重点）
  - string 额外操作
- 🟡：容器适配器 stack queue priority_queue，看懂接口即可
- 读完校验：能回答：什么时候选 list、什么时候 deque、为什么 vector 是默认首选

### Ch10 泛型算法（Generic Algorithms）⭐【课程巨大缺口】

> 
> Frank 几乎没有系统讲 <algorithm>，只零星用过 sort

- 精读小节：
  - 算法基础：迭代器区间 `[begin, end)` 模型、只读算法、写算法、重排算法
  - `find / accumulate / sort / unique / for_each`
  - 谓词 predicate、lambda 表达式（C++11 lambda，重点）
  - bind 绑定适配器
- 重点理解：**算法和容器分离的设计哲学（STL核心思想）**，算法不依赖容器，只依赖迭代器
- 习题：必做，大量手写 lambda + 算法组合
- 读完校验：可以独立写 sort 的自定义比较、unique 去重套路

### Ch11 关联容器（Associative Containers）⭐

> 
> Frank 大概率只是简单提一句 map，没有深入

- 精读：`map / set`（有序，红黑树）、`multimap/multiset`；key 的比较规则
- 🟡：unordered_map / unordered_set 哈希表（冲突、哈希函数、相等判断，第一遍看懂差异，不深挖哈希底层）
- 对比记忆：有序关联容器 vs 无序哈希容器，QPS、有序性、key约束区别（后端面试高频）

### Ch12 动态内存与智能指针（Dynamic Memory）⭐

> 
> Frank 讲过 unique_ptr / shared_ptr，但没有系统梳理裸指针内存模型、内存泄漏、生命周期

- 精读小节：
  - `new / delete`、动态分配数组
  - 内存问题：悬垂指针、内存泄漏、重复 delete
  - **shared_ptr 引用计数、shared_from_this、循环引用（weak_ptr 解决，重中之重）**
  - unique_ptr、move 转移所有权、auto_ptr 废弃原因
  - 分配器 allocator（理解概念即可，不用深挖底层）
- 读完校验：能画出 shared_ptr 循环引用场景 + weak_ptr 解决方案

## Part III 类相关高级特性（Advanced Class Features）

### Ch13 拷贝控制（Copy Control）⭐【移动语义系统化，Frank 课的重点，但零散】

> 
> Frank 讲过 copy / move ctor、move assignment，但 Primer 是教科书级严谨定义

- 精读：
  - 拷贝构造、拷贝赋值、析构
  - 移动构造、移动赋值、`std::move`、右值引用
  - 合成版本什么时候生成、什么时候被抑制（非常多面试题来源）
  - 移动不会抛异常：`noexcept`
  - 三法则、五法则
- 这一章建议慢读，配合手写资源管理类（比如简单的Buffer类）
- 读完校验：可以独立写出一个正确支持拷贝+移动的资源管理类，说出什么时候移动会回退成拷贝

### Ch14 重载运算与类型转换（Overloaded Operations and Conversions）⭐

> 
> Frank 运算符重载案例有，但规则、友元重载、成员重载、转换运算符讲得浅

- 精读：
  - 什么时候成员重载、什么时候非成员友元重载（比如 `operator<<` 必须非成员）
  - 一元、二元运算符重载、自增自减前置后置区分
  - 类型转换运算符、explicit 抑制隐式转换（大坑）
- 🟡：函数调用运算符 `operator()`（functor，和lambda关联起来）
- 读完校验：写一个完整的复数类，支持 + - << 重载，理解 explicit 的意义

### Ch15 面向对象程序设计（Object-Oriented Programming）⭐

> 
> Frank 讲继承、多态，但虚函数、继承层次、访问控制、切片讲得不够严谨

- 精读：
  - 基类 & 派生类、继承 public/protected/private 继承差异
  - **virtual 虚函数、动态绑定、override / final（C++11）**
  - 派生类构造析构顺序、对象切片（slicing，高频坑）
  - 纯虚函数、抽象基类
- 🟡：虚继承 virtual inheritance（菱形继承，第一遍看懂问题，不深度钻；校招常问但工程尽量规避）
- 读完校验：能解释切片现象、虚表的高层模型（Primer不挖底层对象模型，底层留给《深度探索C++对象模型》）

### Ch16 模板与泛型编程（Templates and Generic Programming）⭐【课程几乎空白】

> 
> Frank 几乎没有系统讲模板，只可能零星看到一点

- 精读小节（第一遍不求精通，建立模型）：
  - 函数模板、类模板、模板实例化
  - 模板参数、类型参数、非类型模板参数
  - 模板特化（简单全特化）
- 🟡：可变参数模板、SFINAE 第一遍跳过，属于进阶内容
- 目标：看懂 STL 容器、算法为什么用模板，能写简单模板函数/模板类，不追求写复杂元编程

## Part IV 高级主题（Advanced Topics）🟡【第一遍 Primer 通读时，浅读，标记，不深挖，留到第二轮 / 进阶书】

> 
> 这些 Frank 课程几乎没有覆盖，但 Primer 放在后面，适合**读完主干之后回头啃，面试/工程需要再深耕**

- Ch17 标准库特殊设施：tuple、bitset、随机数、chrono时间库
- Ch18 工具：异常、命名空间、多重继承、虚继承
- Ch19 特殊工具与技术：RTTI、type_info、位域、volatile、内存分配、union

---

# 配套学习清单（读完 Primer 之后，按顺序）

1. 进阶课程：James Raynard 《Modern C++: From Intermediate to Advanced》（正好承接你 Frank 的入门课）
2. 面试补集：《Effective C++》、《Effective Modern C++》（C++11/14 现代最佳实践）
3. 底层模型：《深度探索C++对象模型》（虚表、内存布局，C++服务端面试）
4. STL底层：《STL源码剖析》（侯捷，容器、算法源码）
5. 并发：C++并发编程实战（服务端后端方向）

# 自测验收清单（Primer读完，全部能独立回答=主线通关）

- const顶层底层区分、左值右值、引用绑定规则
- 三法则/五法则、移动语义、std::move、noexcept
- 迭代器失效场景、vector扩容
- STL容器选型：vector/deque/list/map/unordered_map
- 泛型算法 + lambda + 谓词
- shared_ptr循环引用 + weak_ptr
- 虚函数、动态绑定、对象切片、override/final
- 函数重载决议、返回局部引用的未定义行为
- 模板基础：函数模板、类模板、实例化

# 阅读避坑提醒

1. 不要从头匀速一页页啃：前7章、9–16是主线，17–19第一遍浅看；
2. 不要只看书不写代码：C++ 很多概念**只有写错一次才能理解边界**；
3. Frank 的课偏「能跑起来」，Primer 负责「为什么这么写是正确、安全、标准规定」，这就是解决你「知识点片段、不成体系」的核心；
4. Primer 是 C++11，你课程接触了 C++17，读完之后单独整理一份 C++14/17 特性清单（structured binding、if constexpr、fold expression 等）做补丁。

如果你想要，我可以再输出一份：**按天拆分的每日计划表（每天哪几节、做哪几道题、配套小作业）**，或者配套生成一份 Addrnote 站点用的 VitePress 版本文档。
