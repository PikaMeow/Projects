### 一些课程、兴趣驱动的小项目

包括：

1. 课程项目
2. 参与竞赛、科研时做的项目
2. 网上看到的一些有意思的小项目，学习了之后自己实现。

### Genetic Algorithm Implementation

`2016.04` `cpp`

参加「2016年“高教社”杯全国大学生数学建模竞赛」备赛练习时遇到的“任务车间调度问题”。使用C++实现了**遗传算法**，没有依赖遗传算法相关的库。文件`thesis.pdf`描述了算法的思路，博文<a href="http://ycoronene.github.io/2016/04/26/Write-article/" target="_blank">是论文的写作感想</a>。

### Calculator - C++

`2016.06` `cpp`

《编译原理》课程的大作业，写一个计算器的词法分析、语法分析和语义分析部分。博文<a href="http://ycoronene.github.io/2016/06/01/Compiler-Homework-Calculator/" target="_blank">Calculator</a>中描述了问题要求和分析过程，主要是语法分析里FOLLOW集合的生成。

代码注释较少。完整代码为`lexical_2.0.h`和`parse_3.0.cpp`，其余都是实现过程中的半成品。直接编译运行`parse_3.0.cpp`即可,默认从文件`in`中读取字符。

### Calculator - JAVA

`2016.12` `java`

《编译原理实践》课程的大作业，实现一个简单的计算器。

使用了强大的<a href="https://www.antlr.org/" target="_blank">Anltr</a>解析器。直接使用了Antlr强大的功能来做底层的处理。根据计算器较简单的输入规则，实现Anltr API要求的一些方法。


### NoteScanner

`2016.09` `python`

<a href="https://mzucker.github.io/2016/09/20/noteshrink.html" target="_blank">Compressing and enhancing hand-written notes</a> | <a href="https://github.com/mzucker/noteshrink" target="_blank">Github Repo</a>

一个处理手写笔记的程序，通过处理每个像素点的RGB值来增强手写笔迹的清晰度，类似于Office Lens，感觉代码的思路非常清晰，而且还有一些numpy的小技巧。

自己实现了一个简易的版本，一次只能读取一个文件，通过参数`-f`指定读取文件(路径)名，输出名称通过参数`-o`指定，默认格式为**.png**。直接运行.py脚本即可(依赖numpy,PIL和scipy库)，具体参数说明和程序注解可以见博文<a href="http://ycoronene.github.io/2016/09/25/Hand-Written-Notes/" target="_blank"></a>，也可以执行脚本时通过参数`-h`查看帮助。

### SimpleGame 

`2015.12` `python`

<a href="https://realpython.com/blog/python/pygame-a-primer/" target="_blank">PyGame - a Primer</a> | <a href="https://github.com/realpython/pygame-primer" target="_blank">Github Repo</a>

一个PyGame库的使用入门，介绍了其中的一些基本组件和用途，然后应用这些基本组件写了一个很简单的“打飞机”的游戏。

自己重新实现的时候发现作者有几个小地方讲的不清楚，在代码里稍做了修改。同时添加了一些注释。直接运行`.py`脚本即可(依赖PyGame库)。

