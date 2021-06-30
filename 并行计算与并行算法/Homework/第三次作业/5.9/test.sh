# !/bin/sh
rm 5.9.log 5.9.o.log

for p in `seq 1 100`;
do
    mpiexec -n ${p} ./5.9 >> 5.9.log 1000000
done

cat 5.9.log | grep time | awk '{print $4}' >> 5.9.o.log 
