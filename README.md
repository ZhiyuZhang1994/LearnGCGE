### 一、GCGE配置编译

#### 1. 下载外部依赖包
GCGE当前依赖的外部包如下：
- mingw-w64-x86_64-msmpi 10.1.1-11
- mingw-w64-x86_64-openblas 0.3.28-1
- mingw-w64-x86_64-gcc 14.2.0-1(其中自带openmp)

windows平台下msys2环境：采用`pacman -S mingw-w64-x86_64-msmpi 10.1.1-11`进行安装，其他包类似

#### 2. 编译
```bash
cd LearnGCGE
mkdir build
cd build
cmake ..
make 
```

#### 3. 配置编译过程遇见的问题
编译时不能链接libgomp.a和libmingwthrd.a
--解决过程一：
  1.卸载测试用电脑已有的mingw64(排查问题时发现测试用电脑已经安装了mingw64)
  2.参照《BeFEM快速入门手册.pdf》中的Windows下开发环境配置安装了其它的库包
  3.按照“2.编译”流程编译可行

--解决过程二(再次配置编译)：
  1.已卸载测试用电脑已有的mingw64
  2.按照“2.编译”流程编译可行



### 二、运行

#### 1、采用MPI运行方式
编译前(默认设置为MPI形式，因此无需修改)：
 - 修改`gcge\include\ops_config.h`中`#define  OPS_USE_MPI       0`值为1
 - 取消`CMakeLists.txt`中注释` # include(linux_mpicc)`

运行时：
 - 在Mingw64命令行窗口中执行如下命令, 其中<num>为期望采用的进程数量
    ```bash
    mpiexec -n <num> ./test.exe K.mtx M.mtx
    ```

#### 2、非MPI形成运行(需参照2.1内容做反向修改)
在Mingw64命令行窗口中执行:

```bash
./test.exe K.mtx M.mtx
```

#### 3. 运行过程遇见的问题
运行时不能使用mpiexec命令
--原因：mingw-w64-x86_64-msmpi安装包的部分版本不含msmpi
--措施：在windows系统下载安装msmpisetup.exe




# GCGE文件结构

app

config

src

include

# GCGE配置编译

## 外部包

1. 基础线性代数包 BLAS和LAPACK

目前调用的是openblas，如果调用其他库自行修改CMakeLists.txt

2. UMFPACK

直接法解法器，可以加速收敛

## 编译

```bash
cd GCGE1.0_linux/
mkdir build
cd build
cmake ..
make 
```

生成的库文件及头文件见GCGE1.0_linux/shared

## 调用

示例程序路径 GCGE1.0_linux/example

在CMakeLists.txt中已包含对该算例的编译，生成的可执行文件为GCGE1.0_linux/build/test

执行

```bash
./test test1.mtx
```

其中test1.mtx 即为提供的测试算例，补充了上三角部分（标准稀疏矩阵库mtx格式）


## 备注

该算例条件数较大，且矩阵比较病态，目前设置的参数并不适合一般矩阵的特征值问题（会影响效率）；

GCGE1.0_linux/include/ops_config.h 中包含该软件包涉及的一些计算环境的宏定义，注意同步修改这些宏定义；

目前包里提供的mtx转CSC的程序是通用的，注意矩阵文件需要是完整的；

计算结果已与主流特征值解法器SLEPC进行对比，即便是后面较大量级的特征值，在对应精度范围内小数点都是能对上的，且1的代数重数也是一样的


