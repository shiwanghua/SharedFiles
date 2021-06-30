# !/bin/sh
rm 1.1.n.log 1.1.p.log 1.1.MPI.n.log 1.1.Swh.n.log 1.1.MPI.p.log 1.1.Swh.p.log 1.1.p2.log 1.1.Swh.p2.log 1.1.MPI.p2.log 

# p=16

# mpiexec -n ${p} ./1.1  1  >> 1.1.n.log    # 8B
# mpiexec -n ${p} ./1.1  128  >> 1.1.n.log  # 1KB
# mpiexec -n ${p} ./1.1  131072  >> 1.1.n.log # 1MB
# mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 64 / 8` >> 1.1.n.log  # 64MB
# mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 256 / 8`  >> 1.1.n.log # 256MB
# mpiexec -n ${p} ./1.1  `expr 1024 \* 1024 \* 512 / 8`  >> 1.1.n.log # 512MB

# for i in `seq 1 29`;
# do
#     mpiexec -n ${i} ./1.1   `expr 1024 \* 1024 \* 256 / 8` >> 1.1.p.log  
#     echo ${i} processes done
# done

# cat 1.1.n.log | grep MPI | awk '{print $3}' >> 1.1.MPI.n.log 
# cat 1.1.n.log | grep Swh | awk '{print $3}' >> 1.1.Swh.n.log
# cat 1.1.p.log | grep MPI | awk '{print $3}' >> 1.1.MPI.p.log 
# cat 1.1.p.log | grep Swh | awk '{print $3}' >> 1.1.Swh.p.log


for i in `seq 1 32`;
do
    mpiexec -n ${i} ./1.1   `expr 1024 \* 1024 \* 256 / 8 / ${i}` >> 1.1.p2.log  
    echo ${i} processes done
done

cat 1.1.p2.log | grep Swh | awk '{print $3}' >> 1.1.Swh.p2.log
cat 1.1.p2.log | grep MPI | awk '{print $3}' >> 1.1.MPI.p2.log 