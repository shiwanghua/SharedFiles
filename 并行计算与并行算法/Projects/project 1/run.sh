#! /bin/bash

cd 1.1
echo -e "\n\n\nProject 1.1 MPI_ALLGATHER"
make
# 131072指每个进程传输131072个随机生成的double型数据(1MB)
mpiexec -n 2 -f config ./1.1  131072
cd ..

cd 1.2
echo -e "\n\n\nProject 1.2 Gemm"
make
# 4个参数：a矩阵的行数，a矩阵的列数，b矩阵的列数，kernel size
# 随机生成矩阵
# 矩阵乘法，结果保存在 a1.txt b^T.txt ab.txt 三个文件里
# 矩阵卷积，结果保存在 a2.txt k.txt ak.txt 三个文件里
# 矩阵池化，结果保存在 a3.txt a_max_pooling.txt 两个文件里
mpiexec -n 10 -f config ./1.2  1024 1024 1024 4
cd ..

cd 1.3
echo -e "\n\n\nProject 1.3 Wordcout"
make
# 需保证原始文件夹 big_file 和 small_file 和 1.3.c 放在同一目录 ./1.3/ 下
# 大文件的计数结果在 big_file/final_result.txt 下
# 小文件的计数结果在 small_file/final_result.txt 下
mpiexec -n 3 ./1.3
echo -e "\n\n\n"
cd ..