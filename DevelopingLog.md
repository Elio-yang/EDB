# DevelopingLog

- 2021/02/09

1.开发通用的双链表结构

2.cmake编译方法: 需要加入编译的子目录下编写CMakeLists.txt;
新建build文件夹;在build文件夹目录下输入命令

```cmake
cmake .. -G "MinGW Makefiles"
```
在检测各项配置无误之后，便会生成Makefile,此时输入命令
```shell
mingw32-make
```
便可以编译该项目工程。也可以直接
```cmake
cmake ..
```
会调用MSVC编译器，生成符合VisualStudio的解决方案。

3.开发了简单的log系统

- 2021/02/19

1. 宏 alignof

2. 宏 assert

3. 宏 array_size



