# !/bin/sh
rm 6.13.log 6.13.r10.o.log  6.13.rc.o.log  6.13.p.o.log  6.13.t.o.log 


# # 行倍增
# mpiexec -n 10 ./6.13  1000 1000 10 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 100 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 1000 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 10000 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 100000 10 >> 6.13.log 
# echo exp1 done
# cat 6.13.log | grep Time | awk '{print $3}' >> 6.13.r10.o.log 
# rm 6.13.log 


# # 行列互换
# mpiexec -n 10 ./6.13  1000 1000 10 100 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 100 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 10 1000 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 1000 10 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 100 1000 >> 6.13.log 
# mpiexec -n 10 ./6.13  1000 1000 1000 100 >> 6.13.log 
# echo exp2 done
# cat 6.13.log | grep Time | awk '{print $3}' >> 6.13.rc.o.log 
# rm 6.13.log 


# # 进程个数
# p=1
# for i in `seq 1 40`;
# do
#     mpiexec -n ${i} ./6.13  1000 1000 1000 1000 >> 6.13.log  
#     echo ${i} processes done
# done
# echo exp3 done
# cat 6.13.log | grep Time | awk '{print $3}' >> 6.13.p.o.log 
# rm 6.13.log 


# 迭代次数
t=1
for i in `seq 1 5`;
do
    t=$((${t}*10))
    mpiexec -n 10 ./6.13  ${t} ${t} 500 500 >> 6.13.log 
done
echo exp4 done
cat 6.13.log | grep Time | awk '{print $3}' >> 6.13.t.o.log 


