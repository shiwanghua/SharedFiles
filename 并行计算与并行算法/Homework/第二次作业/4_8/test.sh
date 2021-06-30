# !/bin/sh
rm 4.8.log 4.8.o.log

for p in `seq 1 100`;
do
    mpiexec -n ${p} ./4_8_3 1000000 >> 4.8.log
done

cat 4.8.log | grep elapsed | awk '{print $4}' >> 4.8.o.log 
