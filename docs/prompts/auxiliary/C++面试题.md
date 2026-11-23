## 高频TOP 50（按面试出现热度排序）

### 一、C++基础 & 语法（1–18，最高频）
1. `const` 全系列：`const int*` / `int *const` / `const int *const`、const 成员函数、const_cast、const 与 mutable
2. 四种C++强转：`static_cast / dynamic_cast / const_cast / reinterpret_cast` 区别、适用场景、UB风险
3. 指针 & 引用区别；引用底层实现；右值引用、左值、xvalue、prvalue、glvalue；值类别
4. 移动语义、std::move、std::forward 完美转发；万能引用
5. 虚函数、vtable、vptr；虚继承、菱形继承问题与解决方案；虚函数底层布局
6. `sizeof` / `alignof`；内存对齐、padding（填充）；结构体内存布局
7. `auto`、`decltype`、`decltype(auto)` 区别
8. 智能指针：`unique_ptr / shared_ptr / weak_ptr`；shared_ptr 循环引用、control block；自定义删除器
9. 栈、堆、全局/静态存储区；内存分区；静态变量、全局变量生命周期
10. `new / delete` 和 `malloc / free` 的区别；operator new 重载；placement new
11. 拷贝构造、拷贝赋值、移动构造、移动赋值；三法则 / 五法则（Rule of Three / Five）
12. 模板：函数模板、类模板；模板特化、偏特化；SFINAE
13. lambda 表达式：捕获列表、this捕获、mutable、lambda 闭包类型
14. `inline` 关键字；inline函数、inline变量；头文件inline的作用
15. 命名空间namespace；ADL（参数依赖查找）
16. `volatile` 关键字，什么时候用；和const的组合；多线程下误区（volatile≠线程安全）
17. `enum class` 强类型枚举 vs 普通enum
18. constexpr、consteval、constinit 区别；编译期计算

### 二、STL（19–28，后端必考）
19. vector底层：扩容机制、2倍/1.5倍扩容、reserve、resize、shrink_to_fit；迭代器失效
20. string底层、短字符串优化 SSO；string::npos
21. list、vector、deque 对比；各自适用场景
22. map / unordered_map 底层：红黑树 vs 哈希表；哈希冲突解决、负载因子、rehash
23. set / unordered_set；有序容器比较器、key不可修改原因
24. priority_queue 底层；大顶堆、小顶堆
25. 迭代器失效问题（vector、map、list分别什么时候失效）
26. allocator 简单介绍；STL内存池
27. std::bind、std::function
28. 容器适配器：stack、queue

### 三、多线程 & 并发（29–37，后端/游戏服务端重中之重）
29. 进程、线程、协程区别；上下文切换开销
30. 互斥锁mutex、std::lock_guard、std::unique_lock；try_lock
31. condition_variable 条件变量；虚假唤醒、为什么while循环判断而不是if
32. 原子变量 std::atomic；内存序 memory_order（relaxed、acquire、release、acq_rel、seq_cst）
33. 死锁：四个必要条件、避免死锁方法
34. 线程池实现思路；任务队列、任务提交、关闭策略
35. 信号量 semaphore；读写锁shared_mutex
36. 线程局部存储 thread_local
37. 同步原语选型：什么时候用atomic、mutex、rwlock、cond

### 四、操作系统 & 网络（38–46，C++后端必问）
38. 虚拟内存、页表、缺页中断；MMU；COW写时复制
39. 进程地址空间；fork、vfork、exec；fork为什么两次返回
40. 孤儿进程、僵尸进程；wait / waitpid
41. TCP三次握手、四次挥手；TIME_WAIT、CLOSE_WAIT；为什么三次握手
42. TCP粘包拆包；解决方案
43. TCP滑动窗口、拥塞控制
44. IO模型：阻塞IO、非阻塞IO、IO多路复用select/poll/epoll；epoll水平触发LT、边缘触发ET
45. Reactor 模式；单Reactor、多Reactor；Proactor
46. 内存缓存、页缓存、DMA

### 五、工程、底层、系统设计 & 杂项（47–50）
47. 内存泄漏怎么排查；valgrind、asan；野指针、悬垂指针
48. 编译链接四阶段：预处理、编译、汇编、链接；头文件重复包含、static、extern、ODR单定义规则
49. 日志框架设计、高性能日志怎么做到低延迟无锁
50. 你写的C++项目介绍、难点、优化点（必问开放题，所有面试最后必考）
