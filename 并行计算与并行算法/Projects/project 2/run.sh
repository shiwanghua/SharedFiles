#! /bin/bash

cd 2.1
echo -e "\n\n\nProject 2.1 Monte Carlo"
make
# 5个进程 1000000个点
./2.1 5 1000000
cd ..

cd 2.2
echo -e "\n\n\nProject 2.2 Quick Sort"
make
# 1个参数：数组长度
# 输出sorted=1表示结果是正确升序排列的，sorted=0表示结果是错误的
./2.2 1000000
cd ..

cd 2.3
echo -e "\n\n\nProject 2.3 PageRank"
make
# 页数 单页最大链接数 进程数
./2.3 1024000 10 20
echo -e "\n\n\n"
cd ..