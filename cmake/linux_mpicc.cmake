# MPI 
find_package(MPI REQUIRED)
if (MPI_FOUND)
    message(STATUS "MPI found, enabling support.")
    include_directories(${MPI_INCLUDE_PATH})
    # 不需要手动设置编译器，find_package(MPI) 会自动设置
    # set(CMAKE_C_COMPILER ${MPI_C_COMPILER})
    # set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
endif()

# complie flag
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC  -std=c99")