# C++ 后端/游戏服务端面试大纲（完整版，在你原有清单基础上扩充、结构化，覆盖高频+冷门深挖点）

> 
> 结构分为 6 大模块：基础语法 & 类型系统、面向对象 & 内存模型、模板泛型、STL 容器与算法、并发同步、工程实践 & 底层系统 & C++11~C++23 新特性；题号延续你原来的编号，新增内容追加，方便你整理到 Addrnote。
> 标注【面试深挖】代表常追问、容易翻车的点；【UB】未定义行为；【游戏服务端高频】游戏后端重点。

## 一、C++ 基础 & 语法（原1–18，扩充至 1–26）

### 1 const 全系列【必背】

- `const int* p`：指向常量，不能通过 `*p` 修改值；指针本身可改
- `int *const p`：常量指针，指针本身不可改；指向的值可以修改
- `const int *const p`：指针、指向内容都不可修改
- const 成员函数：`void func() const`；this 指针变成 `const T*`；内部不能修改非 mutable 成员；可被 const 对象调用
- mutable：仅用于类成员，允许在 const 成员函数内修改该成员（日志标记、缓存统计场景）
- const_cast：唯一可以去掉 const/volatile 限定符的强转；**不能用来修改原本定义为 const 的对象 → UB**；典型场景：函数入参 const，但老接口需要非 const
- const 与引用、const 右值引用；const 重载（成员函数重载两套：const & / non-const &）
【面试深挖】const 是编译期检查，不是运行期内存保护；rom 段的 const 全局变量修改直接崩溃。

### 2 四种 C++ 强制类型转换

- `static_cast`：基础类型转换、向上转型（父类←子类）、void* 互转；编译期检查；**不做运行时类型检查；向下转型不安全，UB**
- `dynamic_cast`：带运行时类型信息 RTTI；只能用于含有虚函数的多态类型；向下转型失败返回 nullptr（指针）/抛异常 bad_cast（引用）；开销高于 static_cast；不能转 void*
- `const_cast`：仅增减 const / volatile
- `reinterpret_cast`：二进制比特原样重解释；指针↔整数、不同类型指针互转；**完全无安全检查，极度容易 UB**；常见：底层协议、地址映射；禁止业务代码随便用
【面试深挖】为什么不推荐 C 风格强转 `(T)x`：等价于自动选上面四种之一，风险不可控。

### 3 指针 & 引用；值类别系统（左值/右值）

- 指针：独立变量，存地址；可空、可重定向、多级指针；占内存
- 引用：**别名，不是独立变量**；底层编译器大多用指针实现（但是标准不规定！不能说“引用就是指针”）；定义时必须绑定、不能空、不能重绑定；无引用的引用（`int &&&` 折叠规则）
- 值类别【核心】
  - glvalue（泛左值）：有身份（有内存地址），包含 lvalue + xvalue
  - lvalue（左值）：有身份，不能被移动；变量、函数返回左值引用
  - prvalue（纯右值）：无身份；临时对象、字面量、返回值（非引用返回）
  - xvalue（将亡值）：有身份，可以被移动；`std::move(obj)`、返回右值引用 `T&&` 的成员访问
- 引用折叠规则：`T& & → T&`，`T& && → T&`，`T&& & → T&`，`T&& && → T&&`（万能引用的基础）
【面试深挖】区分：**右值引用 `T&&` ≠ 万能引用；万能引用必须搭配模板类型推导 `template<typename T> void f(T&&)`**

### 4 移动语义、std::move、std::forward、完美转发、万能引用

- std::move：不是移动，只是**无条件把参数转为右值引用**；底层是 `static_cast<T&&>`；本身不产生任何代码；move 之后源对象进入合法但未指定状态（只能析构、赋值）
- 万能引用：模板参数 `T&&`，依赖类型推导；根据实参是左/右值，推导成左值引用或右值引用
- std::forward：有条件转换；保留实参原本的值类别，实现完美转发；`forward<T>(arg)`；需要保留推导的 T 信息
- 移动构造 / 移动赋值：接管资源（堆内存、句柄），避免深拷贝；noexcept 对容器性能影响很大（vector 扩容时优先选 noexcept 的移动构造，否则回退拷贝）
【面试深挖】为什么完美转发需要 forward，move 不行；move 后使用对象的坑；noexcept 与异常规格。

### 5 虚函数、vtable、vptr、菱形继承、虚继承

- 虚函数：关键字 virtual；触发动态绑定（运行时决议）；非虚函数静态绑定
- vtable（虚函数表）：每个含有虚函数的类一张；存虚函数地址、RTTI 信息；存在只读数据段
- vptr（虚指针）：每个实例对象开头增加一个指针，指向所属类的 vtable；**增加对象内存开销**
- 菱形继承：B、C 继承 A；D 同时继承 B、C；D 里面存在两份 A 子对象，二义性、数据冗余
- 虚继承 `class B : virtual public A`：共享同一份 A 虚基类子对象；底层靠虚基表 vbptr/vbtable；开销更大、布局更复杂
- 纯虚函数 `virtual void f() = 0`；抽象类，不能实例化；纯虚函数可以有实现
- 析构函数：基类析构必须 virtual，否则 delete 基类指针指向子类对象 → UB（子类部分不析构）
【面试深挖】虚函数能不能 inline？语法允许，但动态调用时不会 inline；构造函数能不能是虚函数？不行；虚函数的调用开销、cache 不友好【游戏服务端高频】。

### 6 sizeof / alignof、内存对齐、padding、对象内存布局

- sizeof：编译期求值；求对象所占字节（包含 padding）；数组 sizeof 得到总长度；引用 sizeof 等于原类型；函数不可 sizeof
- alignof：对齐要求（alignment requirement）；对齐是硬件要求，CPU 访问未对齐内存会变慢、甚至崩溃
- 对齐规则：成员起始地址必须是自身对齐值的整数倍；结构体整体对齐值 = 所有成员最大对齐值；末尾补 padding
- 空类：sizeof 是空类 = 1（C++ 要求每个对象有唯一地址，不能 0 字节）；空类继承、空基类优化 EBO（Empty Base Optimization）【高频】
- 含有虚函数的类：开头 vptr；多重继承、虚继承布局变化
【面试深挖】#pragma pack 改变对齐；alignas 指定对齐；align_val_t；new 的对齐分配。

### 7 auto、decltype、decltype(auto)

- auto：按**初始化表达式的值类别做类型推导**；会丢弃顶层 const &；`auto &&` 万能引用
- decltype(expr)：推导表达式**声明类型**，不求值；区分 decltype((var)) 和 decltype(var)：括号多加一层变成左值引用
- decltype(auto)：C++14；把 auto 的推导规则换成 decltype 规则；用于转发返回值，保留引用；适合封装函数返回

```
auto f1() { return var; }        // 值
decltype(auto) f2() { return var; } // 保留引用
```

【面试深挖】auto 推导数组、函数；auto 不能单独声明函数参数（C++20 auto 参数是简写模板）

### 8 智能指针：unique_ptr / shared_ptr / weak_ptr、控制块、自定义删除器

- unique_ptr：独占所有权；移动语义，禁止拷贝；轻量，开销≈裸指针；支持数组版本 `unique_ptr<T[]>`；默认 delete
- shared_ptr：共享所有权；引用计数；**控制块 control block 和对象本身分离**（make_shared 把对象+控制块分配在一块内存，减少一次堆分配，更优）
- 循环引用：A 持有 shared_ptr<B>，B 持有 shared_ptr<A> → 引用计数永远不为0，内存泄漏；解决方案 weak_ptr
- weak_ptr：弱引用，不增加 ref count；不拥有对象；需要 lock() 提升成 shared_ptr，提升失败返回空（对象已销毁）；专门解决循环引用、缓存观察者
- 自定义删除器：unique_ptr 删除器是类型一部分（会影响类型）；shared_ptr 删除器存在控制块里，不影响类型（更灵活）
【面试深挖】make_shared 缺点：对象销毁后控制块内存要等到所有 weak_ptr 全部释放才回收；shared_ptr 原子操作（refcnt 增减是原子）；线程安全边界：shared_ptr 本身读写不同步，引用计数原子。

### 9 内存分区：栈、堆、全局/静态区、常量区、代码段

- 栈 stack：函数局部变量、函数参数；自动分配释放；向下增长；容量很小（MB级别）；出作用域自动析构；栈溢出
- 堆 heap：new/malloc 分配；手动释放；向上增长；大内存；碎片问题
- 全局/静态存储区：全局变量、static 局部变量；程序启动分配、进程结束释放；static 局部变量只初始化一次（线程安全初始化 C++11 magic statics）
- 只读常量区：字符串字面量、const 全局常量；写操作 UB
- 代码段 .text：二进制指令，只读
【面试深挖】static 局部变量初始化线程安全；static 类成员变量属于类，不属于实例；static 成员函数没有 this。

### 10 new / delete vs malloc / free；operator new；placement new

- malloc/free：C库函数；分配原始字节；不调用构造/析构；失败返回 NULL；无类型
- new/delete：C++表达式；分配内存 + 调用构造；析构 + 释放内存；失败默认抛 bad_alloc；`new[] / delete[]` 配对
- operator new：底层分配函数，可全局/类内重载；只分配内存，不构造；和 new 表达式不是一回事
- placement new：`new (addr) T()`；在一块已经分配好的内存上构造对象；**没有分配内存！**；需要手动显式调用析构函数销毁，不能直接 delete
【面试深挖】delete[] 为什么要记录元素个数；重载 operator new 场景（内存池）；对齐 new (C++17)。

### 11 拷贝构造、拷贝赋值、移动构造、移动赋值；三法则/五法则

- 拷贝构造：同类型对象初始化；传值入参、函数返回值（RVO）；隐式生成版本做逐成员拷贝
- 拷贝赋值：`operator=`，对象已经存在，赋值
- 移动构造、移动赋值：接管资源
- Rule of Three（C++03）：如果你自定义了 析构 / 拷贝构造 / 拷贝赋值 任意一个，三个都应该自定义
- Rule of Five（C++11）：新增移动构造、移动赋值；自定义其一，五个都要考虑
- Rule of Zero（现代C++推荐）：尽量不手写这五个；用 RAII、智能指针管理资源，编译器默认生成版本就正确
【面试深挖】自赋值检测；拷贝赋值经典 copy-and-swap 写法；RVO/NRVO 返回值优化，会跳过拷贝/移动。

### 12 模板：函数模板、类模板、特化、偏特化、SFINAE

- 函数模板、类模板；模板实例化：编译期按需生成代码；代码膨胀问题
- 全特化：模板参数全部确定；独立版本
- 偏特化：只针对部分模板参数（类模板支持偏特化；函数模板不支持偏特化！【高频坑】）
- SFINAE：Substitution Failure Is Not An Error；替换失败不是错误；模板推导失败直接丢弃候选，不编译报错；老式写法 `enable_if`；C++20 concept 替代
【面试深挖】模板是编译期；模板分离编译问题（声明定义放头文件）；非类型模板参数。

### 13 lambda：捕获列表、this、mutable、闭包类型

- 捕获：`[=]` 值捕获、`[&]` 引用捕获、`[this]`、`[*this]`（值捕获this，拷贝当前对象 C++17）、指定捕获
- mutable：默认 lambda operator() 是 const；mutable 允许修改值捕获进来的副本
- 闭包类型：编译器生成唯一匿名类；可转换为函数指针（无捕获lambda）；有捕获不能转函数指针
- 泛型 lambda：`auto` 参数（本质简写模板）C++14
【面试深挖】lambda 生命周期陷阱：引用捕获局部变量，lambda 逃出作用域悬垂引用；lambda 作为回调。

### 14 inline 关键字、inline 函数、inline 变量

- inline 函数：提示编译器展开（只是提示，不一定真inline）；核心作用：**允许头文件定义，多链接单元重复定义，链接器合并，多重定义不报错（ODR 单一定义规则）**
- inline 变量 C++17：全局/静态 inline 变量，头文件定义，多个 .cpp include 不会多重定义；适合头文件常量
- inline 不等于性能一定变好；过度inline增大二进制，指令缓存失效
【面试深挖】ODR；constexpr 函数隐式 inline。

### 15 namespace；ADL 参数依赖查找

- 命名空间防止命名冲突；嵌套命名空间、别名
- ADL（Argument Dependent Lookup，Koenig查找）：函数查找时，额外去实参类型所在命名空间找函数；`std::swap` 特化依赖ADL；`using std::swap` 写法
【面试深挖】为什么推荐 `swap(a,b)` 而不是 std::swap；ADL 坑点。

### 16 volatile 关键字；volatile ≠ 线程安全

- volatile：告诉编译器**不要优化读写，每次必须从内存读取，不能缓存到寄存器**
- 适用场景：内存映射硬件寄存器、信号处理程序中被异步修改的全局变量
- ❌ 误区：volatile 不能保证原子性、不能防止指令重排、**不能替代 mutex / atomic，不等于线程安全**
- const volatile：寄存器只读场景
【面试深挖】现代C++并发几乎不用 volatile；和内存序区分开。

### 17 enum class 强类型枚举 vs 普通 enum

- 普通 enum：隐式转整型；枚举值泄漏到外层作用域；底层类型编译器自选
- enum class（scoped enum）：强类型，不会隐式转 int；名字在枚举域内；可以显式指定底层类型 `enum class E : uint8_t`；类型安全
【面试深挖】如何转成整数；流式输出。

### 18 constexpr / consteval / constinit

- constexpr：编译期可求值，也可以运行期求值；constexpr 函数可以同时在编译期、运行期调用；constexpr 对象编译期常量
- consteval（C++20）：**必须编译期求值，只能产生常量；调用失败直接编译报错**（立即函数）
- constinit（C++20）：保证静态变量在编译期初始化，禁止动态初始化；用来规避静态初始化顺序灾难 SIOF
【面试深挖】constexpr 函数内可以分支循环；constexpr 和 const 区别；constexpr 虚函数 C++20。

> 
> 【新增基础 19–26】

### 19 初始化语法：值初始化、默认初始化、拷贝初始化、列表初始化 {}

- 列表初始化（统一初始化 C++11 `{}`）；窄化转换禁止；`T{}` 值初始化（零初始化）；`T()`
- 聚合初始化、聚合类型；
- 初始化顺序：按成员声明顺序，不是初始化列表顺序！【高频坑】
- 静态初始化顺序灾难 SIOF：不同翻译单元全局静态变量初始化顺序不确定

### 20 悬垂引用 / 野指针；悬空风险场景

野指针：指向已经释放内存的指针；悬垂引用：绑定到已经销毁对象的引用；常见：返回局部变量引用、move后继续使用、裸指针管理堆对象。

### 21 函数返回值优化 RVO / NRVO

RVO：返回临时对象；NRVO：返回局部命名对象；编译器允许直接在调用方栈上构造对象，消除拷贝/移动；C++17 强制拷贝消除（guaranteed copy elision）

### 22 异常：throw / try / catch；noexcept；异常规格

noexcept：标记函数不会抛异常；是编译期标记；`noexcept(expr)` 编译期判断；容器移动、swap 大量依赖 noexcept 判断；异常开销；栈展开 unwind；std::exception

### 23 回调方案对比：函数指针、std::function、lambda、bind

函数指针：轻量，不能捕获；std::function：类型擦除，可保存可调用对象（lambda、bind、函数指针）；有少量开销；

### 24 左值引用成员 & 生命周期延长

const 左值引用可以延长临时对象生命周期（仅直接绑定，链式调用不延长）

```
const T& r = T{}; // 安全；生命周期延长到r作用域结束
const T& r = f().a; // 不延长，悬垂！
```

### 25 this 指针：隐含参数；this 类型；const成员函数 this 是 const T*

### 26 ODR 单一定义规则：什么情况允许多处定义（inline、模板、constexpr）

---

## 二、STL（原19–28，扩充到 27–40）

### 27 vector 底层、扩容、reserve / resize、迭代器失效

- 连续内存数组；`capacity` 容量 vs `size` 元素个数
- 扩容：不同实现 1.5倍 / 2倍；扩容开辟新内存，元素拷贝/移动，释放旧内存；开销大
- reserve：只预分配容量，不创建元素，size不变；resize：改变size，新增元素默认构造，多余销毁
- shrink_to_fit：请求释放多余容量，不保证一定生效
- 迭代器失效：扩容一定全部失效；erase/pop_back：被删位置之后全部失效
【面试深挖】vector<bool> 是位压缩特化，不是容器，坑很多，尽量不用。

### 28 string 底层、SSO 短字符串优化、string::npos

- SSO（Small String Optimization）：短字符串（一般15字节内）直接存在对象内部缓冲区，不分配堆；长字符串才堆分配；不同libstdc++/libc++实现不一样
- string::npos：`size_t(-1)`，无符号最大值；find 找不到返回 npos；判断坑：不能用 int 接收
- +=、append；c_str() 保证末尾 '\0'；data() C++11 之后等价 c_str()

### 29 vector / list / deque 对比 & 场景

- vector：连续内存；随机访问 O(1)；尾部插入摊销O(1)；中间插入删除O(n)；cache友好；适合顺序遍历、随机访问
- list：双向链表；节点分散；随机访问 O(n)；任意位置插入删除 O(1)（拿到迭代器）；cache极差；极少用
- deque：分段连续块数组；双端 O(1) push/pop；支持随机访问（略慢vector）；中间插入删除慢；queue 默认底层容器
【面试深挖】为什么 stack/queue 是容器适配器不是容器

### 30 map / unordered_map：红黑树 vs 哈希表；哈希冲突、负载因子、rehash

- std::map：红黑树（平衡BST）；有序；key 唯一；查找 O(log n)；key 需要 operator<；稳定有序；
- unordered_map：哈希表；数组+链表/开放寻址；无序；平均 O(1) 查找；最坏 O(n)；key 需要 hash 函数 + ==
- 哈希冲突：链地址法（gcc）、开放寻址；负载因子 load_factor；到达阈值触发 rehash：开辟更大桶数组，全部元素重新哈希迁移，开销巨大
【面试深挖】自定义类型作为 unordered_map key，需要特化 hash；map key 不能修改（会破坏有序）；unordered_map erase 会不会 rehash？不会

### 31 set / unordered_set；有序容器比较器

set：红黑树，key就是值，不可重复；multiset/multimap允许重复；自定义比较器；有序容器key是const，禁止修改，破坏排序结构。

### 32 priority_queue 底层：堆；大顶堆、小顶堆

容器适配器，默认 vector + 大顶堆；底层 make_heap/push_heap/pop_heap；堆结构是数组；top是堆顶；pop只删堆顶；小顶堆写法 `greater<T>`。

### 33 迭代器失效总表【后端必背】

- vector：insert/扩容 → 全部失效；erase → 当前位置往后全部失效
- deque：中间 insert/erase 全部失效；头尾插入一般不失效（扩容除外）
- list / forward_list：insert、erase 只有被删除迭代器失效；其余迭代器保持有效
- map / set / unordered_map：erase 仅被删迭代器失效；insert 不失效（map红黑树；unordered_map rehash会全部失效！）
【面试深挖】为什么 unordered_map insert 触发 rehash 迭代器全部失效

### 34 allocator；STL内存池思想

allocator：STL默认内存分配器，封装new/delete；标准allocator无池；老SGI STL有二级内存池；现代C++一般自己实现池化（游戏服务端高频）

### 35 std::bind / std::function

std::bind：绑定函数参数、占位符 `_1 _2`；可以重排参数顺序；bind 生成的对象可以存入 function；std::function 是类型擦除，有堆分配开销（小对象优化SBO）

### 36 容器适配器 stack / queue

stack：默认deque；后进先出；queue：默认deque；先进先出；没有迭代器，不支持遍历

### 37 STL算法：std::sort；std::stable_sort；partition

std::sort： introsort（快速排序+堆排序+插入排序），不稳定；stable_sort归并排序，稳定，需要额外内存；lambda比较器；严格弱序【高频坑】

### 38 std::optional / std::variant / std::any（C++17）

optional：包裹可能有/无的值；代替裸指针返回空；variant：类型安全联合体；any：任意类型容器，类型擦除

### 39 std::tuple、std::pair、结构化绑定 structured bindings C++17

### 40 string_view C++17：不持有内存，只读视图；零拷贝；坑点：生命周期必须比源字符串短

---

## 三、多线程 & 并发（原29–37，扩充至41–56，游戏服务端重中之重）

### 41 进程、线程、协程区别；上下文切换开销

- 进程：资源隔离（独立地址空间）；进程间切换开销大（页表刷新、TLB失效）；IPC通信成本高
- 线程：同一进程内，共享地址空间；切换开销更小；共享数据同步竞争
- 协程（C++20 coroutine）：用户态轻量；不抢占；切换只保存少量寄存器，无内核陷入；开销远低于线程；需要调度器；
【面试深挖】协程不是线程；异步IO、rpc、网关大量使用协程【游戏服务端高频】

### 42 std::mutex、lock_guard、unique_lock、try_lock

- mutex：互斥锁，独占；不可递归；std::recursive_mutex 递归锁（可同一线程多次lock，慎用，开销大）
- std::lock_guard：RAII，构造lock，析构unlock；不可移动、不可解锁；最简单
- std::unique_lock：更灵活；支持延迟锁、手动unlock、移动、和cond搭配；占用略重；条件变量必须用unique_lock
- try_lock：非阻塞加锁，拿到返回true，拿不到立刻返回false；防止死锁

### 43 condition_variable 条件变量；虚假唤醒；while vs if

- cond cv：配合mutex；等待条件；wait会自动unlock mutex + 阻塞；唤醒后重新lock
- 虚假唤醒：spurious wakeup；即使没人notify，wait也可能返回；**必须 while 判断条件，绝对不能 if**
- notify_one / notify_all；notify 不持有锁也可以，但一般建议锁内通知（业务逻辑）
【面试深挖】为什么会虚假唤醒？标准允许；底层操作系统实现原因

### 44 std::atomic；内存序 memory_order【并发天花板考点】

原子变量：无锁同步；CPU原子指令；不需要mutex；支持整型、指针；不支持自定义复杂类型（默认）
内存序从松到严：

1. memory_order_relaxed：只保证原子性，无任何重排约束；计数器
2. memory_order_acquire：读，后面指令不能跑到 acquire 前面
3. memory_order_release：写，前面指令不能跑到 release 后面
4. memory_order_acq_rel：同时 acquire + release（read-modify-write，如fetch_add）
5. memory_order_seq_cst：顺序一致，全局总序；默认，开销最大
【面试深挖】release-acquire 同步链；happens-before；synchronizes-with；内存屏障；ABA问题

### 45 死锁：四个必要条件、避免死锁方法

四大条件（同时满足才会死锁）

1. 互斥：资源独占
2. 持有并等待：已经持有锁，再申请新锁
3. 不可剥夺：锁不能被别人抢走
4. 循环等待：锁形成环路
规避方案：固定锁顺序、统一加锁顺序、try_lock超时、减少锁粒度、避免嵌套锁

### 46 线程池实现思路；任务队列、关闭策略

组成：任务队列（queue + mutex + cond）、一组工作线程、提交接口、停止标记

- 提交：任务push队列，notify；
- 关闭：两种模式：① 等队列任务全部做完再退出（优雅关闭）；② 丢弃剩余任务；原子stop标记；join所有线程；
- 扩展：任务优先级队列、任务限流、批量提交；
【游戏服务端高频】线程池隔离：IO线程池、逻辑线程池分开

### 47 信号量 semaphore（C++20 std::counting_semaphore）

计数信号量；资源计数；P操作acquire、V操作release；可以大于1个资源；比cond更适合资源池限流；二进制信号量≈锁（不等同）

### 48 读写锁 shared_mutex（C++17）

shared_mutex：共享读、独占写；读多写少场景；

- shared_lock：读锁，多个读者并行
- unique_lock：写锁，独占；写的时候所有读阻塞；
⚠️ 坑：写饥饿；写优先/读优先策略；写操作优先级设计

### 49 thread_local 线程局部存储

每个线程独立一份变量；线程创建分配，线程销毁回收；不共享；适合每个线程独立缓存、随机种子；不能跨线程访问；
【面试深挖】thread_local 初始化时机；线程退出析构；开销

### 50 同步原语选型对比表（面试高频问答题）

- 单纯计数器、标记、标志：atomic + relaxed/acquire-release，无锁最快
- 短临界区、独占修改：mutex + lock_guard
- 读远多于写：shared_mutex读写锁
- 等待某个条件成立：condition_variable + mutex
- 资源数量限制、限流：semaphore
- 复杂异步调度：协程

### 51 std::thread；join / detach；joinable；线程异常

join：主线程阻塞等待子线程完成；detach：分离，线程后台跑，主线程不再管；detach风险：线程访问局部变量悬垂；

### 52 future / promise / packaged_task / async

std::promise：设置值；std::future：获取异步结果；get()阻塞；std::packaged_task：包装可调用对象，绑定future；std::async：启动异步任务；launch::async / launch::deferred（延迟执行）
【面试深挖】future共享状态；shared_future；

### 53 无锁编程基础：ABA问题；解决：版本号、 hazard pointer

### 54 屏障 std::barrier、std::latch（C++20）：一组线程同步等待点

### 55 并发数据结构设计思路：锁粒度（粗锁、细粒度锁、无锁）

### 56 C++20 协程基础：co_await / co_yield / co_return；promise_type；awaiter【游戏服务端、网关、RPC 热门】

---

## 四、面向对象设计 & 设计模式（后端面试追加模块，57–65）

### 57 三大特性：封装、继承、多态

多态分为静态多态（重载、模板）、动态多态（虚函数）

### 58 访问控制 public / protected / private；protected 继承含义

public继承：is-a；protected继承、private继承（has-a，组合优先）

### 59 继承 vs 组合：优先组合，少用继承；继承耦合强，组合低耦合

### 60 单例模式；多种写法（懒汉、饿汉、magic static C++11 线程安全单例）；单例优缺点

### 61 RAII 资源获取即初始化（C++核心范式！锁、智能指针、文件句柄全部基于RAII）

### 62 开闭原则、里氏替换LSP、依赖倒置SOLID简要

### 63 工厂模式、观察者模式（游戏事件系统高频）

### 64 CRTP 奇异递归模板模式（静态多态，Mixin组件，游戏组件系统高频）

### 65 type traits std::is_pointer / is_base_of / enable_if；C++20 concept

---

## 五、底层系统 & 网络相关（游戏服务端/C++后端高频拓展，66–75）

> 
> 很多C++面试最后会问到系统、网络，和C++代码结合

### 66 内存池、对象池；碎片问题；固定大小池、多级池

### 67 套接字、IO模型：阻塞IO、非阻塞IO、select/poll/epoll；LT水平触发 ET边缘触发【游戏网关】

### 68 Reactor 模式；Proactor；one loop per thread

### 69 TCP 粘包拆包；分包方案（长度头）

### 70 定时器实现：红黑树定时器、时间轮（游戏服务器定时任务）

### 71 日志系统设计；无锁日志队列

### 72 内存泄漏检测工具：valgrind、asan；内存越界；use-after-free

### 73 CPU cache、cache line、伪共享 false sharing；多线程填充padding规避伪共享【高性能服务高频】

### 74 大端小端；网络字节序 htons/ntohl

### 75 信号 signal；信号处理函数限制；不能在信号函数里调用大部分std库

---

## 六、C++版本演进 & 工程规范（76–82）

### 76 C++11：右值引用/move、lambda、智能指针、thread、atomic、统一初始化、magic static

### 77 C++14：decltype(auto)、泛型lambda、shared_timed_mutex

### 78 C++17：inline变量、structured bindings、string_view、optional、variant、any、guaranteed copy elision、shared_mutex

### 79 C++20：concept、consteval、constinit、协程、std::semaphore、barrier、latch、range-for增强、module模块

### 80 C++23：deducing this、std::expected、range算法增强

### 81 头文件规范；#include守卫 #pragma once vs #ifndef

### 82 编译链接流程：预处理→编译→汇编→链接；静态库.a、动态库.so；符号解析、重定位

