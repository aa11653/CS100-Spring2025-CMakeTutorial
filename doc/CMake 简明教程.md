文稿/配图：张中瀚
2025.6.3

*本教程以 Windows 为主要参考的操作系统，VSCode 为主要参考的代码编辑器，MinGW GCC 为主要参考的编译器。*

*本教程起源于在 C++ 学习过程中囫囵吞枣走的一些弯路，因此，本教程致力于由浅入深，从简单工具讲到上层配置，尽可能覆盖 C++ 项目的常见术语，希望能给各位带来一些帮助。*

本教程的相关代码见 [GitHub: aa11653/CS100-Spring2025-CMakeTutorial](https://github.com/aa11653/CS100-Spring2025-CMakeTutorial)

# C++ 项目

- C++ 源码的编译过程回顾
	- 头文件、源代码、中间的二进制文件、可执行文件
- C++ 生成目标：库，或者可执行文件
- 处理依赖：作为分发者或使用者

## 例子：洛伦兹吸引子轨迹计算器

下面，我们通过一个简单的例子，**来快速上手 C++ 项目里的一些概念**。

### 背景：洛伦兹吸引子

> 洛伦兹吸引子（Lorenz Attractor）最初由爱德华·洛伦兹（Edward Lorenz）于1963年在简化的大气对流模型中提出，它揭示了确定性系统中“蝴蝶效应”所体现的混沌行为。
> 
> 简单来说，一个粒子在以 $\sigma, \rho, \beta$ 为参数的洛伦兹系统里的任意时刻的运动速度可以由以下方程组描述
> $$ \frac{d}{dt} \left[ \begin{matrix}
x(t) \\
y(t) \\
z(t)
\end{matrix} \right] = \left[ \begin{matrix}
\sigma(y-z) \\
x(\rho-z)-y \\
xy-\beta z
\end{matrix} \right] := \vec{F}(x,y,z) $$
> 如果我们给定一个初始位置，并按照等式右侧描述的速度对粒子进行追踪，我们就能对粒子的运动进行模拟。而“混沌”现象则指的是对于一批粒子，即使他们的初始位置十分接近，一段时间后它们仍然总会无序的分散到空间里。

<table>
  <tr>
    <td><img src="assets/LorenzVisual1.png" style="width:16em;height:16em" /></td>
    <td><img src="assets/LorenzVisual2.png" style="width:16em;height:16em" /></td>
    <td><img src="assets/LorenzVisual3.png" style="width:16em;height:16em" /></td>
  </tr>
</table>

*图：初始位置相近的例子，在受洛伦兹吸引子影响一段时间后会分散在空间中。*
### 要做的事

我们将要编写一个“洛伦兹吸引子”的工具组件，包含一个**计算库**和一个**命令行的轨迹生成器**。
- **计算库:** 它提供了**洛伦兹场的对象**，使用者可以计算任一点场的数值（粒子的速度）或者旋度（可以换算到粒子的旋转速度）
- **轨迹生成器:** 它是一个**命令行程序**，用户输入初始位置后，它可以计算出 10s 内粒子的轨迹

## 动起手来写代码

接下来，我们快速完成这个项目的代码。
- `MyVector.h, MyVector.cpp`：首先，因为洛伦兹场 $\vec{F}$ 是一个 $\mathbb{R}^3\to\mathbb{R}^3$ 的映射，方便起见，**我们先搓一个最小的向量运算库** 
- `LorenzField.h, LorenzField.cpp`：然后，我们编写**另一个库**，它包含一个描述洛伦兹场的对象，它根据参数初始化一个洛伦兹场，随后，我们便可以通过 `LorenzField::valueAt` 或 `LorenzField::curlAt` 计算场上某位置的速度或旋度
- `TraceQuery.cpp`：最后，我们编写一个**可执行程序**，它接受用户传入的初始点，在场内追踪这个粒子，生成一条粒子的轨迹。

> [!INFO] 概念：库（Library）、可执行程序（Executable）
> 库是**供其他程序调用的代码集合**，通常用于封装一组相关的功能模块或算法逻辑。它的用户通常是程序员。程序员编写代码，通过调用库的一系列函数与它进行交互。
> 
> 可执行程序是**可以被操作系统直接运行的程序文件**，例如 `.exe` 文件（Windows）或无扩展名的 Unix 可执行文件。它的用户可以是任何人，甚至大多是时候使用者不需要有编程背景。用户通过图形界面 (GUI) 或命令行界面 (CLI) 进行输入，并得到他们想要的结果。

**以下哪些是库，哪些是可执行程序？**

- `gcc`，我们常用的编译器
- `matplotlib`，python 绘图包
- `numpy`，一套数值计算工具
- Chrome / Microsoft Edge，一系列浏览器
- `Selenium`[（项目链接）](https://www.selenium.dev/documentation/webdriver/getting_started/first_script/) ，一个浏览器自动化需要使用的东西

## 写代码时的依赖关系

在我们写代码的时候，VSCode 会跳出各种各样的提示，帮助我们进行补全、检查错误……例如下图里，我们敲下成员访问的 `.` 号，VSCode 会自动提示 `position` 作为 `Vector3` 会有 `.x()`, `.y()`, `.z()` 三个“成员函数”。

![[VSCode 的语言解析服务.png]]

那 VSCode 是怎么知道的呢？头文件！

```cpp
/// main.c
#include "LorenzField.h"

/// LorenzField.h
#include "MyVector.h"
```

如上所示，我们引入了*同目录下的*洛伦兹场头
→ 而洛伦兹场头又引入了*同目录下的*向量头
→ 向量头里写着 `MyVector` 类的所有方法的声明

这样 VSCode（准确来说是编辑器指挥的语言解析服务程序）顺藤摸瓜，就把所有方法找到了。

>[!INFO] 头文件的作用：写代码的依赖，或“开发时的依赖”
>**作为库的使用者**，引入头文件，也就是引入头文件内阐述的一系列声明，会对程序开发提供很好的帮助。
>
>**作为库的编写者**，我们最终需要把这个库的用法（类的声明、函数的声明、乃至全局变量的声明……）放到头文件里，以供其他人引用。

## 编译过程

>[!INFO] 概念：广义的编译过程 (Compilation Process)、编译目标 (Compilation Target)

我们编译这一个小玩具很简单，只需要一行命令， GCC 就能帮我们搞定一切。
```
g++ .\TraceQuery.cpp .\LorenzField.cpp -o TraceQuery.exe
```

### 对于更大的项目

但是，为解决更大的项目做准备，我们需要知道屋檐之下发生了什么。具体来说，编译并运行一个程序大致分成以下步骤：

1. 我们把每一个翻译单元的**源码**，逐个翻译成**目标文件 (object file)**。每个目标文件里，都有源代码对应的实现。
2. 我们把一部分**目标文件**文件链接成**静态链接库**或**动态链接库**。
3. 然后，我们将带有**主函数 `main(...)` 的目标文件**，与**部分其他目标文件 + 静态链接库**静态链接，形成单一的**可执行文件**。
4. 最后，在**可执行文件**加载时，操作系统找到**动态链接库**，然后进入主函数。

在这个过程里，每个静态库或动态库都包含了一套可用的工具，它们可以被其他程序利用；最终的可执行程序可以直接运行。**因此，我们将静态库、动态库、可执行程序统称为编译目标 (target)。**

![[Cpp 项目的编译和运行.excalidraw|100%]]

*更准确地说，我们通常说的“编译并运行”分成了编译、汇编、链接、加载四个步骤。这点我们会在未来的 CS110（[Computer Architecture, Week 4](https://toast-lab.sist.shanghaitech.edu.cn/courses/CS110@ShanghaiTech/Spring-2024/index.html)）中详细学习。*

### 对此项目而言……

像这个只有两个编译单元的项目情况，就有以下三种“编译并运行”方式。*图注：橙色部分为文本文件，蓝色部分为紧凑的二进制文件。*

![[Cpp 此项目的编译.excalidraw|100%]]
以下列出了这三种方式的命令行指令。

```bash
# (Part I) 创建一个承放产物的文件夹
mkdir build
cd build

# (Part II) 调用 g++ 生成目标文件         
g++ -c ..\LorenzField.cpp -o LorenzField.o
g++ -c ..\TraceQuery.cpp -o TraceQuery.o

# (Part III) 将目标文件链接成库，将库和目标文件链接成可执行文件
# 方式 (1)
g++ TraceQuery.o LorenzField.o -o LorenzField.exe
# 方式 (2)
ar rcs libLorenz.a LorenzField.o
g++ TraceQuery.o -L. -lLorenz -o TraceQuery.exe
# g++ ../TraceQuery.cpp -L. -lLorenz -o TraceQuery.exe # 试一试
# 方式 (3)
# ...
```

前面两行，我们创建了一个 `build/` 文件夹，因为接下来我们会生成很多的中间产物，我们不想它们和源码堆在一起。

然后接下来的三行，我们使用 `g++` 将我们的**源码转换成目标文件**。现在，让我们具体看一看这些目标文件吧。目标文件是紧凑的二进制格式，里面用机器语言记录着这段源代码需要进行的操作。因为它们已经成为二进制文件了，用记事本或编辑器打开只能得到一串乱码，但是用二进制文件查看器或许可以看出一些端倪。

![[VSCode HexEditor 打开目标文件.png|400]]
*图：LorenzField.o 是紧凑的二进制文件，这是使用 VSCode 的 Hex Editor 打开的效果。*


最后，我们按照三种方法来完成收尾。

*方法1:* 直接用 `g++` 链接两个目标，得到单一的可执行文件。
*方法2:* 用 `g++` 将 `LorenzField.o` 生成为静态库 `libLorenz.a`，然后再把静态库和含有 `main()` 的目标链接，得到单一的可执行文件。
*方法3:* 先生成动态链接库，然后生成可执行程序。这种方式在 Windows 操作系统下比较复杂，但在 Linux 操作系统下相对简单。因此这里不详细展开，只给出一些参考链接，供读者感兴趣时阅读。

- [Windows, Cygwin 文档: Building and Using DLLs](https://cygwin.com/cygwin-ug-net/dll.html)
- [Linux, Stack Overflow 论坛回答: Build .so file from .c file using gcc command line](https://stackoverflow.com/questions/14884126/build-so-file-from-c-file-using-gcc-command-line)

此外，如果你用记事本打开可执行文件 / 库文件的话，也只能得到一串乱码。它们也是紧凑的二进制格式的。

## 可执行文件 & 库的分发与使用

程序或库的分发或使用对象可以大致二分为
1. 开发者（与源码接触的人）
2. 普通用户（只与二进制接触的人）

根据上文的编码和编译过程，我们可以总结出向他们分发可执行文件或库的方式，以及我们使用可执行文件或库的方式。

>[!INFO] 可执行文件 & 库的分发
> 如果我们要**分发*静态链接的*可执行文件**
> - 我们直接分发 `TraceQuery.exe` 即可
> - 也可以分发所有源码与二进制依赖让用户自己构建
> 
> 如果我们要**分发库**，
> - 如果接受者有二进制文件，我们只需要分发二进制文件即可。用户拿到手一链接就成
> - 如果接受者有是源文件，我们有两种选择：(1) 二进制文件 + 头文件 (2) 源文件 + 头文件

>[!INFO] 可执行文件 & 库的使用
> 如果我们**要用别人写好的*静态链接的*软件**
> - 直接下载 `.exe` 即可
> - 也可以下载所有源码与二进制依赖并自己构建
>
> 如果我们**要用别人写好库**：
> - 我们的程序要用？下载二进制文件
> - 我们的源码要用？下载二进制文件和头

生活例子 1：OBS Studio 是一款好用的开源的录屏和推流软件，开发者将它自己托管在 Github 上，
- 这个项目更接近是程序？还是库？
- [Release](https://github.com/obsproject/obs-studio/releases/tag/31.0.3) 下有哪些你认识的分发方式？
- [主页](https://github.com/obsproject/obs-studio?tab=readme-ov-file#quick-links) 下的 Quick Links → Build 是在干什么？

![[OBS Release Example.png]]

生活例子 2：在 Ubuntu 系统（一个 Linux 发行版）中，我们可以通过 `apt` 命令搜索并安装软件包。有时，我们会发现我们能同时搜索出很多名字类似的软件包，例如搜索开源的一个加密工具 `openssl` 时，我们会拿到至少以下三个东西

```
$ apt search openssl

Sorting... Done
Full Text Search... Done
openssl/focal-updates,focal-security 1.1.1f-1ubuntu2.24 amd64 # 第一个东西
  Secure Sockets Layer toolkit - cryptographic utility

libssl-dev/focal-updates,focal-security 1.1.1f-1ubuntu2.24 amd64 # 第二个东西
  Secure Sockets Layer toolkit - development files

libssl1.1/focal-updates,focal-security 1.1.1f-1ubuntu2.24 amd64 # 第三个东西
  Secure Sockets Layer toolkit - shared libraries
```

首先，库还是程序？其次，如果是库，它们的分发方式是什么？
- openssl：**这是 OpenSSL 的命令行工具包**，提供加密和 SSL/TLS 功能。
- libssl-dev：**这是使用头 + 二进制分发的静态库**，面向开发者。具体而言，包含开发 OpenSSL 应用程序所需的头文件和静态库。
- libssl1.1：**只有二进制的共享库**，供其他程序使用。

生活例子 3：另外，有些库本身只有头文件，没有源文件。比如 [glm](https://github.com/g-truc/glm)（一个常见的图形学数学库），这个时候，使用者只需要把头文件下载下来，然后 include 到源码即可，全程不涉及二进制的下载与管理。

![[GLM Example.png]]

## 然而，随着项目越来越大

- 依赖越来越多了……
- 一次编译需要几十、几百、甚至几万条指令……
- 别人的库分散在很多文件夹里，要么就是每一条 `#include` Directive 要走好几层目录，要么每一条编译指令的 `IncludePath` 要写几十行

总而言之，手动编译不再可行了

# CMake

- CMake，是什么？不是什么？
- 下载并配置
- 基本命令
- 最小例子
- 其他参考资料

## CMake 是什么？不是什么？

CMake 是一个跨平台的自动化构建系统，它主要用于生成用于构建软件项目的构建文件（例如 Makefile、Visual Studio 工程文件等），然后让其余构建工具指挥编译器完成项目的编译。CMake 通过提供一个简单的配置文件（`CMakeLists.txt`）来描述项目的构建过程。

![[CMake 使用的工具链.excalidraw|60%]]


**然而，CMake 不是一个集成开发环境（IDE）。** 虽然它可以与许多 IDE 配合使用（如 Visual Studio、Xcode 或 CLion），但它本身并不提供代码编辑、调试等开发环境功能。

**CMake 也不是编译器本身。** 它依赖于实际的编译器（如 GCC、Clang 或 MSVC）来完成代码的编译和构建工作。

## 下载 CMake, 配置 VSCode 插件

**请参考以下链接安装 CMake**

- [菜鸟教程: CMake 安装与配置](https://www.runoob.com/cmake/cmake-install-setup.html)
- [CMake 官网: Download CMake](https://cmake.org/download/)

如果你使用的是 macOS, 你可能需要在执行安装后，参考左上角菜单栏上“How to Install For Command Line Use”指令，将 CMake 添加到命令行。

此外，可以参考 CS100 课程的 Final Project 的具体教程配置 CMake。

**请参考以下链接配置 VSCode 的 CMake 插件**

从以下链接安装插件： [Visual Studio Marketplace: CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

这里再给出一个官方的教程：[VSCode 教程: Get started with CMake Tools on Linux](https://code.visualstudio.com/docs/cpp/CMake-linux)

尽管这篇教程是 Linux 为主的，但除了安装 `gcc` 和 `cmake` 之外的部分，Windows 都基本适用。

此外，可以参考 CS100 课程的 Final Project 的具体教程配置 CMake 的 VSCode 插件。

## 使用 CMake 组织我们的项目

>[!NOTE] 如果这是一个课堂例子，请逐步、带停顿和确认地进行

在开始前，我们需要在心中梳理项目的结构，**什么是可执行的编译目标？什么是库文件编译目标？我们要分发库文件的话，我们还需要一并分发哪些头文件？**

![[CMake 简明教程 项目架构.excalidraw|100%]]

大致梳理一下，我们的项目，不妨叫做 `Lorenz`，共有两个构建目标：
- 目标1 / 库 / LorenzField，包含一个三维向量洛伦兹场的一些工具
- 目标2 / 可执行文件 / TraceQuery，一个命令行界面的，查询例子轨迹的小程序

即使我们没有引入 CMake，维持一个良好的文件组织也是必要的。一种典型的组织方式是：将所有源文件、头文件按照构建目标放入 `Lorenz/src/{{目标名}}` 文件夹。

```
Lorenz/ (项目根目录)
|-- README.md (一个简短的说明文件)
+-- src/ (存放一系列源码文件)
    |-- LorenzField/ (第一个目标的相关文件)
    |   |-- LorenzField.h
    |   |-- LorenzField.cpp
    |   +-- MyVector.h
    +-- TraceQuery/ (第二个目标的相关文件)
        +-- TraceQuery.cpp
```

然后，我们可以在以下地方创建管理此项目几个 `CMakeLists.txt` 了：它位于项目根目录里、存放源码文件的 `src/` 目录下，以及每一个目标 `src/LorenzField` `src/TraceQuery` 目录下。

```txt hl:2,5,7,12
Lorenz/ (项目根目录)
|-- CMakeLists.txt !! 一: 控制项目!!
|-- README.md (一个简短的说明文件)
+-- src/ (存放一系列源码文件)
    |-- CMakeLists.txt !! 二: 坐镇源码 !!
    |-- LorenzField/ (第一个目标的相关文件)
	|   |-- CMakeLists.txt !! 三: 管理 LorenzField 库目标 !!
    |   |-- LorenzField.h
    |   |-- LorenzField.cpp
    |   +-- MyVector.h
    +-- TraceQuery/ (第二个目标的相关文件)
        |-- CMakeLists.txt !! 四: 管理 TraceQuery 可执行程序目标 !!
        +-- TraceQuery.cpp
```

## CMakeLists.txt 其之一：控制项目

它的内容通常很简单：规定 CMake 的版本，设定项目的名称，设定 C++ 版本，最后，指引到坐镇源码的 `CMakeLists.txt`。

```txt title:Lorenz/CMakeLists.txt
cmake_minimum_required(VERSION 3.10) # 规定 CMake 的版本
project(LorenzFieldTracer) # 设定项目的名称
set(CMAKE_CXX_STANDARD 17) # 设定 C++ 版本

add_subdirectory(src) # 指引到 src 的 CMakeLists.txt
```

## CMakeLists.txt 其之二：坐镇源码

它的内容更加简单，指引到两个管理编译目标的 `CMakeLists.txt` 即可

```txt title:Lorenz/src/CMakeLists.txt
add_subdirectory(LorenzField)
add_subdirectory(TraceQuery)
```

## CMakeLists.txt 其之三: 管理 LorenzField 库目标

```txt title:Lorenz/src/LorenzField/CMakeLists.txt
# 添加一个库目标
add_library(LorenzField # 目标的名称，不必与源码的名称相同
    LorenzField.cpp # 目标的源码，可以是用空格/换行隔开的多个文件，相当于传给 g++ 的参数
)

# 管理头文件的目录
target_include_directories(LorenzField # 目标的名称
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src # 目标包含且对外暴露的头文件目录
)
```

### 管理头文件：`target_include_directory(...)`

一个常见的误解是：`target_include_directories(...)` 仅用于声明某个目标（target）所依赖的头文件路径，即<i>仅仅</i>表示“我这个目标需要包含哪些头文件目录”。

但实际上，它的行为取决于其第二个参数，也就是作用域（`PRIVATE`、`PUBLIC`、`INTERFACE`）的设置。在声明为 `PUBLIC` 或 `INTERFACE` 时，这一命令的真正含义<i>还包括</i>**该目标对外暴露哪些头文件路径**。这意味着，当其他目标在链接到这个目标时，会自动继承这些头文件路径。例如，如果一个库对外提供了一组头文件供其他模块使用，就应当通过 `PUBLIC` 或 `INTERFACE` 方式进行暴露，而不是使用 `PRIVATE`。

> [!INFO] `target_include_directory` 的作用域参数
> 总结来说：
> - **`PUBLIC`：当前目标和依赖它的目标都使用这些路径。**
> - `PRIVATE`：仅当前目标使用这些头文件路径。
> - `INTERFACE`：只有依赖它的目标使用这些路径，当前目标本身不需要。

此外，为什么路径写是 `${PROJECT_SOURCE_DIR}/src`？`${PROJECT_SOURCE_DIR}` 是一个自带的变量，指向最近一次调用 `project(...)` 的位置，也就是项目的根目录。进而， `${PROJECT_SOURCE_DIR}/src` 指向项目的 `src/` 目录。当然你可能会好奇，让它指向 `src/LorenzField/` 不好吗？

考虑如果我们在另一个目标引入 `MyVector.h`，我们要怎么写？答案是，我们要写基于`target_include_directories` 给出的目录的相对位置，对于前一种方式，也就是 `LorenzField/MyVector.h`。

```txt hl:3,5,7
Lorenz/
|-- README.md
+-- src/ (${PROJECT_SOURCE_DIR}/src)
    |-- LorenzField/
    |   |-- LorenzField.h (LorenzField/LorenzField.h)
    |   |-- LorenzField.cpp
    |   +-- MyVector.h (LorenzField/MyVector.h)
    +-- TraceQuery/
        +-- TraceQuery.cpp
```

而如果我们要引入一个头文件，我们只能这么写……

```txt hl:1 title=可能是另一个目标的源码.cpp
#include <LorenzField/MyVector.h>
/*...*/
```

这样依赖，我们（被迫）更清晰地表达了“LorenzField 库提供的 MyVector.h”的意图，也更不容易撞名了。

倘若后一种方式，就只能写成 `#include <MyVector.h>`，这样就相对不太清晰了。最后，无论什么情况下使用 `#include "相对当前文件的路径"` 的方式引入头文件总是可用的。

> [!INFO] `target_include_directory` 的目录选择
> 如上文所述，请结合导入头文件的写法的可读性综合考虑

>[!WARNING] 这样设置之后，`TraceQuery.cpp` 中的导入路径需要相应调整

## CMakeLists.txt 其之四: 管理 TraceQuery 可执行目标

```txt title:Lorenz/src/LorenzField/CMakeLists.txt
# 添加一个可执行目标
add_executable(TraceQuery
    TraceQuery.cpp
)

# 添加为 TraceQuery 目标添加 LorenzField 库作为依赖
target_link_libraries(TraceQuery
    LorenzField
)
```

### 引入依赖： `target_link_libraries(...)`

![[CMake 简明教程 项目架构.excalidraw|100%]]

再次回顾我们的项目架构，我们发现：**当我们说 `TraceQuery` 依赖 `LorenzField` 时，我们同时需要它的头文件和二进制库**。好在，CMake 的 `target_link_libraries` 可以帮我们同时做到这一点。因为刚刚讲到了，我们的 `LorenzField` 的引入目录被设置成了“公开的”(即 `PUBLIC`），而链接它的目标会自动继承它的引入目录。

对于这一个小项目，我们就不需要再为 `TraceQuery` 使用 `target_include_directories` 了，因为没有额外的头文件需要引入了。

*此外，`target_link_libraries` 也有作用域设置，但在这里篇幅所限，不展开了。*


>[!INFO] 确保自己理解了以下五条 CMake 基本指令
>
>- `project` 设定项目名称
>- `add_subdirectory` 管理子文件夹
>- `add_executable`, `add_library` 添加编译目标（库或可执行文件）
>- `target_link_library` 引入编译目标的依赖
>- `target_include_directories` 管理库自身依赖或向外分发的头文件的目录

## 配置 (Configure)，构建 (Build)，调试或运行 (Debug or Run)

准备完成！接下来，让我们分三步“编译”我们的最终目标（一个库、一个可执行程序）、并运行场轨迹查询程序吧！
### 配置 (Configure)
<div style="break-before:avoid-page;break-after:avoid-page;"></div>
还记得吗？CMake 不是编译器等具体的“演奏者”，相反，它是“乐队的指挥家”，配置过程，就类似写乐谱的过程。它会生成一系列的构建脚本，提前规划好编译的路径。此外，在配置时，我们需要选择使用何种编译器（被称为 **Kit**）。
<div style="break-before:avoid-page;break-after:avoid-page;"></div>

![[CMake 配置.excalidraw|100%]]

### 构建 (Build)
<div style="break-before:avoid-page;break-after:avoid-page;"></div>
接下来，当我们按下 Build 时，CMake  会按照预先生成的脚本，指挥小弟干活，把最终的目标编译出来。默认情况下，我们会构建所有编译目标。此外，我们也可以选择构建选定的某个（和它以来的）编译目标。然后，点下按钮，开始构建吧。
<div style="break-before:avoid-page;break-after:avoid-page;"></div>

![[CMake 构建.excalidraw|100%]]
### 启动调试/运行
<div style="break-before:avoid-page;break-after:avoid-page;"></div>
最后，可以点击 debug 按钮调试项目，就和以前的单文件项目一样调就行。
<div style="break-before:avoid-page;break-after:avoid-page;"></div>

![[CMake 调试.excalidraw|100%]]

（虽然此项目不涉及）但有的时候，我们需要手动指定运行时的工作目录，请参考这个讨论： [Github: VSCode CMake Tools - Set debugger working directory](https://github.com/microsoft/vscode-cmake-tools/issues/1258)。

## 将第三方库纳入管理的最佳实践

CMake 功能强大，而且在持续更新，通过一篇简明教程把所有事情全讲明白是不可行的。在我们掌握了 C++ 项目的基本架构之后、编译方式，并手动完成一次依赖管理后，剩下读读文档、看看别人大项目里的文件、问问大语言模型，就万变不离其宗了。

### 第一种 使用 [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)

点进官网，我们能看到它的最小例子：只需要轻松几行，就可以如刚刚所说的一样将第三方依赖引入了。

```txt hl:9,10,12-14
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# create project
project(MyProject)

# add executable
add_executable(main main.cpp)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#7.1.3")
CPMAddPackage("gh:nlohmann/json@3.10.5")
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# link dependencies
target_link_libraries(main fmt::fmt nlohmann_json::nlohmann_json Catch2::Catch2WithMain)
```

吴义恒助教的 CMake + CPM 例子：
- 示例地址：[CS100-2025-Spring-Recitations/r14](https://github.com/Nagisaaaaaaaaa/CS100-2025-Spring-Recitations/tree/main/r14) 
- 关于 [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake)：CPM.cmake 是一个跨平台的 CMake 脚本，它增强了 CMake 的第三方依赖项管理功能。可以用它轻松的引入托管在 Github 等地方的依赖。

### 第二种（稍微底层一些）

尽管 CPM 已经非常好用了，但是又是我们不可避免的要处理一些棘手的情况。因此，我们需要在一定程度上了解使用原版 CMake 简述以下管理第三方库的简单流程：

以安装 [Magnum Engine](https://magnum.graphics/) 为例

1. **下载:** 使用直接下载或其他方式，将 `Magnum` 下载到项目根目录的 `third_party/Magnum` 文件夹：
	- 如果项目本身使用 Git 管理的，那么在已有的存储库里进行 `git clone` 不太妥当，更好的方式是使用 [Git - 子模块](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97) 。
	- 当然使用 `git clone` 或直接下载在某种程度上都可以跑得动就是了……
2. **添加子目录:** 使用 `add_subdirectory(third_party/Magnum/CMakeLists.txt)` 添加项目的 CMake 配置文件。
3. （可选）使用 `find_package` 让 CMake 找到最适合的依赖选项：
	- 这需要项目根目录里存在 `modules/findMagnum.cmake`。
	- 找到的库可能是刚刚下载的，也可能是已经安装在计算机的更适合的。具体请参考以下例子、[官方文档](https://cmake.org/cmake/help/latest/command/find_package.html)。此外 [这个回答](https://stackoverflow.com/questions/38081708/should-i-use-add-subdirectory-or-find-package)讲述了为什么使用 `add_subdirectory` 后仍继续 `find_package` 是更好的。
4. **配置链接:** 最后，使用 `target_link_libraries` 将为你的目标添加依赖。

作为本教程的结尾，这里再给出一个实践例子：

Magnum C++ 起步教程：
- Magnum C++ 是一个封装良好的、使用现代 C++ 的可视化引擎。它的起步教程个人认为也是一个很好的 CMake 进阶材料，手把手教你如何使用 CMake 将第三方库纳入管理，并编写相对复杂的图形界面程序。
- 示例地址 [Getting started | Magnum C++ docs](https://doc.magnum.graphics/magnum/getting-started.html)
- 此外，本项目的存储库里，提供一个利用 Magnum 编写的洛伦兹吸引子的可视化示例

> 注：Magnum 的起步教程还是为了简便性做出了一些妥协……
> 
> - 它这里直接 `git clone` 了其他项目作为“子存储库”，然而，更推荐的做法是使用 [Git - 子模块](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97)
> - 它直接将所有第三方依赖下载到项目根目录，然而，更推荐的做法是所有第三方库应该归拢到根目录下的 `third_party/` 或类似名称的文件夹

# 限制

1. C++ 的模板，自身不提供定义，需要进行模板实例化后才会生成定义。因此模板库总是以头文件分发的，不能进行二进制分发。
2. `target_include_directories` 和 `target_add_libraries` 有额外的参数，能力与篇幅有限，还请自行翻阅文档。
3. 受限于文档篇幅，本教程无法容纳命令行、 VSCode、Git、部分 CMake 的详细介绍和一些图形学的前置知识。还请读者自行查询。