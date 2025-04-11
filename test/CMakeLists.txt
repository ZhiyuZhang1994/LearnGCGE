
set(GOOGLETEST_DIR "D:\\BeFem_Dan\\googletest")  # 替换为你安装的实际路径

# 找到 GoogleTest 的头文件和库文件
include_directories(${GOOGLETEST_DIR}/googletest/include)
link_directories(${GOOGLETEST_DIR}/build/lib)  # 如果 GoogleTest 是用 CMake 构建的，这里是编译后的库路径

# 创建测试可执行文件
add_executable(my_test test_slepc.cpp)  # 你的测试文件

# 链接 GoogleTest 和 pthread 库
target_link_libraries(my_test gtest gtest_main pthread)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MPI_C_INCLUDE_PATH}
    ${PETSC_INCLUDE_DIR} # PETSc 头文件路径
    ${SLEPC_INCLUDE_DIR} # SLEPc 头文件路径
)
# 设置库文件搜索路径
link_directories(
    ${PETSC_LIB_DIR}
    ${SLEPC_LIB_DIR}
)

# 注册测试
add_test(NAME MyTest COMMAND my_test)
