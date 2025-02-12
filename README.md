# Nand2tetris
这个项目包含：

1.硬件架构设计与编译器开发

硬件设计：使用硬件描述语言（HDL），从 Nand 门开始，设计出基本逻辑门；在此基础上构建出内存、CPU 等关键组
件，最终整合成一个基础的计算机原型。

汇编器开发：设计并实现了一个汇编器，将汇编语言转换为机器码；包括实现词法分析、语法分析、符号表管理及最终
的机器码生成。

编译器开发：前端模块：将高级语言编译为中间表示。具体工作包括词法分析、语法分析、抽象语法树生成、符号表管
理以及中间代码生成。后端模块：基于虚拟机和运行时环境将中间表示翻译到汇编语言。

2.虚拟机与基础操作系统开发

虚拟机与运行时环境：类似于 Java 的 JVM，开发了一个虚拟机及其运行时环境，为编译器后端提供支持。

操作系统实现：设计并实现了一个基础操作系统，提供系统调用接口，支持标准 I/O、字符串处理、内存管理等。
