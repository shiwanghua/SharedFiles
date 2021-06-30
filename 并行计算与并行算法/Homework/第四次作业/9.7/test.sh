# !/bin/sh
rm 1.1.log 1.1.Swh.o.log 1.1.MPI.o.log 

p=1
for i in `seq 1 40`;
do
    mpiexec -n ${i} ./1.1  100000 >> 1.1.log  
    echo ${i} processes done
done

cat 1.1.log | grep Time | awk '{print $3}' >> 1.1.o.log 



p=32

mpiexec -n ${p} ./1.1  1  >> 1.1.log    # 8B
mpiexec -n ${p} ./1.1  128  >> 1.1.log  # 1KB
mpiexec -n ${p} ./1.1  131072  >> 1.1.log # 1MB
mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 64 / 8` >> 6.10.log  # 64MB
mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 256 / 8`  >> 6.10.log # 256MB
mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 512 / 8`  >> 6.10.log # 512MB
mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 1024 / 8` >> 6.10.log # 1GB


# for i in `seq 0 8`;
# do
#     mpiexec -n ${p} ./6.10  `expr 1024 \* 1024 \* 128 / 8` ${i} >> 6.10.log # 256MB
# done

p=1
for i in `seq 1 6`;
do
    p=$((${p}*2))
    mpiexec -n ${p} ./6.10  `expr 1024 \* 1024 \* 128 / 8` 0 >> 6.10.log # 128MB
done

cat 1.1.log | grep MPI | awk '{print $3}' >> 1.1.MPI.o.log 
cat 1.1.log | grep Swh | awk '{print $3}' >> 1.1.Swh.o.log 
