# !/bin/sh

# without -f
rm 1.3.bf.nf.log 1.3.ba.nf.log 1.3.su.nf.log 1.3.ss.nf.log
for i in `seq 2 51`;
do
    mpiexec -n ${i} ./1.3 >> 1.3.log  
    echo nf ${i} processes done
done

cat 1.3.log | grep FixedLength | awk '{print $6}' >> 1.3.bf.nf.log # big_fixed_nonConfig
cat 1.3.log | grep AvgLength | awk '{print $6}' >> 1.3.ba.nf.log
cat 1.3.log | grep UnSort | awk '{print $6}' >> 1.3.su.nf.log # small_unsort
cat 1.3.log | grep Sorted | awk '{print $6}' >> 1.3.ss.nf.log  # small_sorted_config

rm 1.3.log 

# with -f
rm 1.3.bf.f.log 1.3.ba.f.log 1.3.su.f.log 1.3.ss.f.log
for i in `seq 2 51`;
do
    mpiexec -n ${i} -f config ./1.3 >> 1.3.log  
    echo f ${i} processes done
done

cat 1.3.log | grep FixedLength | awk '{print $6}' >> 1.3.bf.f.log
cat 1.3.log | grep AvgLength | awk '{print $6}' >> 1.3.ba.f.log
cat 1.3.log | grep UnSort | awk '{print $6}' >> 1.3.su.f.log # small_unsort
cat 1.3.log | grep Sorted | awk '{print $6}' >> 1.3.ss.f.log  # small_sorted

rm 1.3.log 1.3.fnf.log
mpiexec -n 5  ./1.3 >> 1.3.log  
mpiexec -n 5 -f config ./1.3 >> 1.3.log  
mpiexec -n 9  ./1.3 >> 1.3.log  
mpiexec -n 9 -f config ./1.3 >> 1.3.log  
mpiexec -n 17  ./1.3 >> 1.3.log  
mpiexec -n 17 -f config ./1.3 >> 1.3.log  
cat 1.3.log |  awk '{print $6}' >> 1.3.fnf.log # 不能有其他信息输出！

