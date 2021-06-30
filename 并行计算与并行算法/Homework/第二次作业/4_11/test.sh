# !/bin/sh
rm 4.11.log 4.11.o.log

for p in `seq 1 100`;
do
    mpiexec -n ${p} ./4.11 >> 4.11.log
done

cat 4.11.log | grep Time | awk '{print $3}' >> 4.11.o.log 
