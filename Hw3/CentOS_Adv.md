<meta charset="utf-8" lang="en"><style class="fallback">body{visibility:hidden;}</style>

**Makefile 文件作用及编写方法**
    1651574 贾昊霖*


# Makefile 文件作用
makefile文件本质目的是实现自动化编译，其作用在于可以使程序员只通过一个命令完成整个工程的编译、链接以至于最后的执行操作

# Makefile 基本语法

## Makefile 的包含
### 指定makefile文件
通常为`makefile` 或 `Makefile`，如其他名称，则用一下命令
``` bush
make -f filename.xxx
```
### 包含其他文件
``` bush
include /home/admin/*.mk
# 或者再输入命令时添加，默认makefile/Makefile
make --include-dir /home/admin
```
这个会类似于C语言，把文件内容复制到当前位置

## 书写规则
### 调用格式
``` bush
targets : prerequisites
    command \
    command # started with \t
    ...
```
target更新规则：
1. 目标文件没有生成
2. 某个prerequisite中的文件修改时间比target文件晚
3. 某个条件中的文件需要更新

### 隐式规则
make工具会自动使用`g++ -c`命令，将扩展名为`.c`的文件编译成一个同名的`.o`目标文件
因此
``` bush
test: main.o
    g++ -o test main.o
main.o: main.c main.h
    g++ -c main.c
clean:
    rm test *.o
```
可简化成
``` bush
test: main.o
    g++ -o test main.o
main.o: main.h
clean:
    rm test *.o
```

### 使用伪目标
例如上例的clean,就是一个伪目标，make指令并不生成clean这个目标文件，并且无法生成其依赖关系，也无法决定该目标是否需要执行。
因此只有通过显式地指明这个目标才能对其进行执行
``` bush
make clean
```
为了避免和文件名冲突，使用特殊标记`.PHONY`来显式指明一个目标是伪目标，例如
``` bush
.PHONY : clean
```
**注意，makefile文件中的第1个目标会被作为默认目标**

### 使用Shell命令
使用`@`字符在命令行前，这个命令将不被make命令显示出来，例如
``` bush
@echo compiling...
```
输出为
``` bush
`compiling...`
```
否则如果这样
``` bush
echo compiling...
```
输出则会变成
``` bush
echo compiling...
compiling...
```
此外，`make -n`可以只是显示命令，但不会执行命令。`make -s`则不现实命令提示，无论前面是否有`@`符

## 使用变量
引用变量时，在变量名前加`$`符号，`$(Variable)`
### 变量定义
- `=`左侧变量，右侧变量地值
- `:=` 避免递归定义的危险，同`=`
- `?=`类似于C++中的`ifndef`
### 追加变量的值
`+=` 类似于`strcat()`函数，或者通过` variable1 = $(variable2) $(variable3)` 的方式


# makefile实例
递归向下寻找所有该路径下的文件夹中的makefile文件，并执行make指令
``` makefile
CC = g++
CFLAGS = -c
DIRS = $(shell find . * -type d | grep -v "\.")

.PHONY = clean test

main: test

test:
	for dir in $(DIRS);do \
		make -C $$dir || echo "with no makefile";\
	done

clean:
	for dir in $(DIRS);do \
		make -C $$dir clean || echo "with no makefile"; \
	done
```
其中 `make -C` 可以指定目录执行make指令，`find . * -type d`将当前目录下的所有子目录找到，`grep -v "\."`为反转寻找，即寻找不含`'.'`的目录

# 压缩与解压缩
## 压缩归档
`tar -cvjf 目标文件 源文件`
- `c`为`tar`的功能，创建一个新的`tar`归档文件
- `vjf`为`tar`的选项
    - `j` 重定向给bzip2命令压缩文件
    - `v` 在处理文件时显示文件
    - `f` 将输出结果到文件或设备`file`
[](./pic/zip-1.png)
## 解压缩提取
`tar -xvjf 目标文件 源文件`
- `x`为`tar`的功能，从已有的`tar`归档文件中提取
[](./pic/zip-2.png)


**程序的静态与动态编译**
# Linux 下的动态编译


# 动态编译
## 静态编译与动态编译的比较
- 动态编译：可执行文件需要附带一个的动态链接库，在执行时，需要调用其对应动态链接库中的命令。所以其优点一方面是缩小了执行文件本身的体积，另一方面是加快了编译速度，节省了系统资源。缺点一是哪怕是很简单的程序，只用到了链接库中的一两条命令，也需要附带一个相对庞大的链接库；二是如果其他计算机上没有安装对应的运行库，则用动态编译的可执行文件就不能运行
- 静态编译：编译器在编译可执行文件的时候，将可执行文件需要调用的对应动态链接库(.so)中的部分提取出来，链接到可执行文件中去，使可执行文件在运行的时候不依赖于动态链接库
## 动态编译
- printf("hello，world") 
    - 用`gcc -o test test.cpp`指令编译
    - 用`wc test` 观察执行文件的字节数
    [](./pic/dy_st_init-1-1.png)
    三个数分别为 行数，词数，字节数
    - 字节数为 8448
- cout << "hello，world"
    - 用`g++ -o test test.cpp`指令编译
    [](./pic/dy_st_init-1-2.png)
    - 字节数为 8888
- mysql_demo.cpp 的动态编译命令
   - 用`g++ $(mysql\_config --cflags) demo.cpp $(mysql\_config --libs) -o out`指令编译
    [](./pic/dy_st_init-1-3.png)
   - 字节数为 14192
## 查找某个可执行文件所依赖的动态链接库
通过`ldd 文件名` 指令查找某可执行文件所依赖的动态链接库
    [](./pic/dy_st_init-1-4.png)


# Linux下的静态编译
## 静态编译
- 先安装静态编译库(保证联网的状态)
``` bush
yum install glibc-static libstdc++-static
```
上图的命令已经将gcc 与g++的静态链接库全安装了
[](./pic/dy_st_init-2-3.png)

- printf("hello，world")
    - 用`gcc -static test.cpp -o test`指令编译，生成可执行文件
    - 用`wc`观察test的字节数
    [](./pic/dy_st_init-2-1.png)
    - 字节数为861152

- cout<<"hello，world"
    - 用`g++ -static test.cpp -o test`指令编译，生成可执行文件
    - 用`wc`观察test的字节数
    [](./pic/dy_st_init-2-2.png)
    - 字节数为1608232

- 编写目录总makefile与上题类似，不再赘述
    最后结果如下图所示
    [](./pic/dy_st_init-2-4.png) [](./pic/dy_st_init-2-5.png)

- 压缩方法与上文所述相同，亦不再赘述



# Linux下的动态链接库的编译与使用

[学习网址1](http://www.cnblogs.com/skynet/p/3372855.html)
[学习网址2](https://www.cnblogs.com/fnlingnzb-learner/p/8059251.html)

## 库的概念
库是写好的现有的，成熟的，可以复用的代码。现实中每个程序都要依赖很多基础的底层库，不可能每个人的代码都从零开始，因此库的存在意义非同寻常。
本质上来说**库是一种可执行代码的二进制形式**，可以被操作系统载入内存执行。库有两种：静态库（.a、.lib）和动态库（.so、.dll）。
其中静态、动态是指**链接**
    [](./pic/dy_st_adv-1-1.png)

## 静态链接库
### 静态链接库的概念
在链接阶段，将汇编生成的目标文件.o与引用到的库一起链接打包到可执行文件中。其对应的链接方式称为静态链接。
因此，既然静态库与汇编生成的目标文件一起链接为可执行文件，那么静态库必定跟.o文件格式相似。其实一个静态库可以简单看成是**一组目标文件（.o/.obj文件）**的集合，即很多目标文件经过压缩打包后形成的一个文件
    [](./pic/dy_st_adv-1-2.png)

### 静态库的特点
- 静态库对函数库的链接是放在编译时期完成的。
- 程序在运行时与函数库再无瓜葛，移植方便。
- 浪费空间和资源，因为所有相关的目标文件与牵涉到的函数库被链接合成一个可执行文件
另外静态库对程序的更新、部署和发布页会带来麻烦。如果静态库`liba.lib`更新了，所以使用它的应用程序都需要重新编译、发布给用户（对于玩家来说，可能是一个很小的改动，却导致整个程序重新下载，全量更新）
    [](./pic/dy_st_adv-1-3.png)

## 动态链接库
### 动态链接库的概念
在程序编译时并不被连接到目标代码中，而是在程序运行是才被载入。不同的应用程序如果调用相同的库，那么在内存里只需要有一份该共享库的实例，规避了空间浪费问题。动态库在程序运行是才被载入，也解决了静态库对程序的更新、部署和发布页会带来麻烦。用户只需要更新动态库即可，增量更新
### 动态库的特点
- 动态库把对一些库函数的链接载入推迟到程序运行的时期。
- 可以实现进程之间的资源共享。（因此动态库也称为共享库）
- 将一些程序升级变得简单。
- 甚至可以真正做到链接载入完全由程序员在程序代码中控制（显示调用）
    [](./pic/dy_st_adv-1-4.png)

## 动态链接库实例
hello.h 头文件
``` C++
#pragma once
#include <iostream>
using namespace std;
void hello();
```
hello.cpp 函数段
``` C++
#include "hello.h"
void hello(){
    cout<<"Hello World!<<endl;
}
```
test.cpp 主程序
``` C++
#include <iostream>  
#include "hello.h"
int main(){  
    cout<<"call function"<<endl;
    hello();  
}  
```
- 首先，生成目标文件，此时要加编译器选项`-fPIC` 与链接器选项`-shared`
``` bush
g++ -fPIC -shared -o libhello.so hello.cpp
```
`-fPIC` 创建与地址无关的编译程序（pic，position independent code），是为了能够在多个应用程序间共享
[](./pic/dy_st_adv-2-1.png)

- 然后，引用动态库编译成可执行文件
``` bush
g++ test.cpp -L. -lhello -o test
```
其中:
`-L` 表示要连接的库所在目录                                                     **## ？？？？还有用么？？？？？？**
`./`或`.` 表示当前目录
`-l` 指定链接时需要的动态库，编译器查找动态连接库时有**隐含的命名规则**，即在给出的名字前面加上lib，后面加上.a或.so来确定库的名称。
[](./pic/dy_st_adv-2-2.png)
发现报错信息，找不到动态链接库

经查询，发现有两种方法可以解决此问题：
1. 如果安装在`/lib`或者`/usr/lib`下，那么ld默认能够找到，无需其他操作，如果没有，可以手动添加至该目录下
    因此，可以将生成的动态链接库文件通过指令`cp libhello.so /usr/lib`拷贝值`/usr/lib`目录下，再运行
2. 如果安装在其他目录，需要将其添加到动态链接`/etc/ld.so.cache`缓存文件中，步骤如下：
    - 编辑`/etc/ld.so.conf`文件，加入所需库文件所在目录的路径
    - 运行`ldconfig`，该命令会重建`/etc/ld.so.cache`文件
本次选择第一种方法：
[](./pic/dy_st_adv-2-3.png)
成功！

## 写动态链接库测试样例

###  gcc的makefile编写
`./01/` 中的makefile如下
``` makefile
CC = gcc
SUFFIX = .c
CFLAGS = -c

test: libtest1.so
	$(CC) test2$(SUFFIX) -L. -ltest1 -o test
    # 当然可以加入下面这句话，避免每次都要刷新缓存
    # ldconfig

libtest1.so: test1$(SUFFIX)
	$(CC) -fPIC -shared -o libtest1.so test1$(SUFFIX)

clean:
	rm -f libtest1.so test
```
本次选用第二种错误解决方法
修改`/etc/ld.so.conf`，并用`idconfig`将`/etc/ld.so.conf`的数据读入高速缓存`/etc/ld.so.cache`中
[](./pic/dy_st_adv-3-2.png)
其中`/etc/ld.so.conf`修改如下：
[](./pic/dy_st_adv-3-1.png)

### g++的makefile编写
同理只要将`./02/`目录下的`makefile`中`SUFFIX`改为`.cpp`，CC改为`g++` 即可正常运行
[](./pic/dy_st_adv-3-4.png)
记得还要在`/etc/ld.so.conf`中加入当前路径
[](./pic/dy_st_adv-3-3.png)




<!-Markdeep: --><style class="fallback">body{visibility:hidden}</style><script src="markdeep.min.js"></script>

<script src="jquery-3.3.1.min.js"></script>

<script type="text/javascript">
    $(document).ready(function(){setTimeout(function(){$(".image a").removeAttr("href")}, 0)});
</script>

<style>
    .md h2::before{
        content: counter(h2, lower-alpha)
    }

    .md h2.notinc::before{
        counter-increment: none;
        content: none
    }

    div.notinc1 h1::before{
        counter-increment: none;
        content: "2*"
    }

    .md h1.notinc::before{
        counter-increment: none;
    }

    .md .image {
        width: 80%;
    }

    .md img {
        border: 1.3px rgb(0, 0, 0) solid;
    }

    .md div.imagecaption {
        text-align: center;
    }

    .level2 .tocnumber {
        display: none
    }

    .md code {
        background-color: lightgoldenrodyellow;
        color: rgb(190, 67, 50);
    }

    .md pre code {
        background: none;
        /* background-color: rgb(250, 238, 224); */
        /* border: 1.0px rgb(0, 0, 0) solid; */
    }

    body {
        font-family: 等线,Palatino,Georgia,"Times New Roman",serif;
    }
</style>