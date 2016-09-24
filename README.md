## 一些日常写的小项目

包括：

1. 从网上看到的别人写的有意思的小项目，学习一下再自己copy实现一下。
2. 想到一些idea，自己摸索着实现一下。

### <a href="https://realpython.com/blog/python/pygame-a-primer/" target="_blank">PyGame - a Primer</a>
<a href="https://github.com/realpython/pygame-primer" target="_blank">Github Repo</a>

一个PyGame库的使用入门，介绍了其中的一些基本组件和用途，然后应用这些基本组件写了一个很简单的“打飞机”的游戏。

自己重新实现的时发现作者有几个小地方讲的不清楚，然后在代码里悄悄地改掉了~~

自己的实现放在了SimpleGame目录下，加了点自我感觉良好的注释。直接运行`.py`脚本即可(依赖PyGame库 -_-!废话)。


### <a href="http://ycoronene.github.io/2016/06/01/Compiler-Homework-Calculator/" target="_blank">Calculator</a>

编译课程的大作业，写一个计算器的词法分析、语法分析和语义分析部分。写了一篇文章描述问题要求和分析过程，主要是语法分析里FOLLOW集合的生成。

实现放在了Calculator目录下，懒得没什么注释。。。有用的代码是`lexical_2.0.h`和`parse_3.0.cpp`，其余都是实现过程中的半成品。直接编译运行`parse_3.0.cpp`即可,默认从文件`in`里读取字符。





